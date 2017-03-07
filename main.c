#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <dlfcn.h>
#include <dirent.h>
#include <string.h>
#include "app_msgs.h"


struct sw *switches[MAX_OBJECTS] = {};
struct protocol *protocols[MAX_OBJECTS] = {};
struct comm *comm_mods[MAX_OBJECTS] = {};
struct cli *cli_mods[MAX_OBJECTS] = {};
int switch_cnt=0;
int proto_cnt=0;
int comm_cnt=0;
int cli_cnt=0;
int exit_main=0;

void *hndl[MAX_OBJECTS] = {};
int hndl_cnt=0;

int register_sw (struct sw *sw_var)
{
    switches[switch_cnt]=sw_var;
    switch_cnt ++;

    return 1;
}

int register_proto (struct protocol *proto_var)
{
    protocols[proto_cnt]=proto_var;
    proto_cnt ++;

    return 1;
}

int register_comm (struct comm *comm_var)
{
    comm_mods[comm_cnt]=comm_var;
    comm_cnt ++;

    return 1;
}

int register_cli (struct cli *cli_var)
{
    cli_mods[cli_cnt]=cli_var;
    cli_cnt ++;

    return 1;
}

int register_mod (int type, void *var)
{
	switch (type) {
		case MOD_PROTOCOL :
                   return register_proto (var);
	           break;
		case MOD_SWITCH :
                   return register_sw (var);
	           break;
		case MOD_COMM :
                   return register_comm (var);
	           break;
		case MOD_CLI :
                   return register_cli (var);
	           break;
		default :
		   printf("Unknown Module Type\n");
	}
	return 1;
}

int read_libs (char * dir_name)
{
   DIR *dir;
   struct dirent *ent;
   if ((dir = opendir (dir_name)) != NULL) {
	  /* print all the files and directories within directory */
	  while ((ent = readdir (dir)) != NULL) {
		     if (strstr(ent->d_name, ".so"))
	             {
                          hndl[hndl_cnt]=dlopen(ent->d_name, RTLD_LAZY);
		          if (!hndl[hndl_cnt])
		          {
	                      perror ("");
		              printf ("unable to open %s %s\n", ent->d_name, dlerror());
		          }
			  else
			  {
		              printf ("opened library %s\n", ent->d_name);
			      hndl_cnt ++;
			  }
		      }
	  }
	    closedir (dir);
   } else {
	  /* could not open directory */
	  perror ("");
	    return 0;
  }
  return 0;
}

int register_dev (void *param)
{
	printf("%s %d\n", __FUNCTION__, __LINE__);
	return 1;
}

int app_msg_qry_sw (void * parameter)
{
	int size=SZ_BUF-sizeof(struct app_msg_hdr);
	int *sw_type;
	int loop;

	printf("App Query %s %d\n", __FUNCTION__, __LINE__);
	printf("parameter %p\n", parameter);
	sw_type=(int *)parameter;
	printf("sw_type %d\n", *sw_type);

	if(*sw_type == MAX_OBJECTS)
	{
	       printf("%s %d\n", __FUNCTION__, __LINE__);
		/* return entire list */
	        for (loop=0; loop<switch_cnt; loop++)
	        {
			*sw_type=switches[loop]->type;
			sw_type ++;
	        }
		return switch_cnt * sizeof(int);
	}	
	else
	{
	       printf("%s %d\n", __FUNCTION__, __LINE__);
		*sw_type=switches[*sw_type]->type;
		return sizeof(int);
	}

	printf("%s %d\n", __FUNCTION__, __LINE__);
	return 1;
}

int app_msg_qry_proto (void * parameter)
{
	printf("App Query Protocols : %s %d\n", __FUNCTION__, __LINE__);
	return 1;
}

struct messages app_message[] = {
             {PROTOCOL_APP,MSG_QRY_SW,app_msg_qry_sw},
             {PROTOCOL_APP,MSG_QRY_PROTO,app_msg_qry_proto}};

struct protocol app_main = {
	"MAIN_APP",
	PROTOCOL_APP,
	0,
	MSG_QRY_MAX,
	app_message,
};

int protocol_app_print()
{
	printf("Main Application : %s %d\n",app_main.name, app_main.type);

   return 1;
}

int cli_cmd (char *cmd_str)
{
   printf("Main application command %s \n", cmd_str);
}


int halt()
{

   printf("Halting Main app\n");

   return 1;
}

int app_init ()
{
    app_main.register_proto = register_dev; 
    app_main.print_func=protocol_app_print;
    app_main.cli_cmd=cli_cmd;
    app_main.halt = halt;
    register_mod (MOD_PROTOCOL, &app_main);
    return 1;
}

/* When we receive a INT signal, halt all subsystems */
void
sigint_handler(int signum)
{
   int loop;
  
   printf("Signal %d\n",signum); 

   if (signum==SIGUSR1){
      exit_main=1;

      signal(SIGINT, SIG_DFL);
      signal(SIGTERM, SIG_DFL);
     // kill(getpid(), SIGINT);
   }

}

void
exit_handler()
{

    static int in_halt=0;
    int loop;

   if (!in_halt) {
        in_halt=1;

        /* Halt the messaging side first */
        printf ("\n\nStopping communications modules \n");
        for (loop=0; loop<comm_cnt; loop ++)
        {
	      (* comm_mods[loop]->halt)();
        }
        printf ("\n\nStopping cli  modules \n");
        for (loop=0; loop<cli_cnt; loop ++)
        {
	      (* cli_mods[loop]->halt)();
        /* Halt the switches  */
        printf ("\n\nStopping switching  modules \n");
        for (loop=0; loop<switch_cnt; loop ++)
        {
	      (* switches[loop]->halt)();
        }
        /* Halt the protocols  */
        printf ("\n\nStopping protocol  modules \n");
        for (loop=0; loop<proto_cnt; loop ++)
        {
	      (* protocols[loop]->halt)();
        }
        }
        for (loop=0;loop<hndl_cnt;loop++)
        {
	        dlclose(hndl[loop]);
        }
      kill(getpid(), SIGKILL);
   }

    
}

int main ()
{
   void *hndl1;
   void *hndl2;
   int loop;
   int proto_loop;
   int (*reg_sw)(int (*register_sw)(int, void *));
   int wstatus;
   exit_main=0;
   

   signal(SIGUSR1, sigint_handler);
   signal(SIGCHLD, sigint_handler);
   signal(SIGTERM, sigint_handler);
   signal(SIGKILL, sigint_handler);
   read_libs ("./.");

   /* Load all libraries */
   for (loop=0;loop<hndl_cnt;loop++)
   {
	   reg_sw = dlsym(hndl[loop], "init");
	   if (reg_sw)
	   {
		(*reg_sw)((*register_mod));
	   }
   }

   app_init();
   /*
    * Now that we've loaded protocols and switches
    * registers the switches with the protocols 
    */
   printf ("Linking Protocols to Switches \n");
   for (proto_loop=0; proto_loop<proto_cnt; proto_loop ++)
   {
       if (protocols[proto_loop]!=NULL) {
           for (loop=0;loop<switch_cnt;loop++)
	   {
	       (* switches[loop]->proto_reg_func)(
			       protocols[proto_loop]->type,
			       protocols[proto_loop]->register_proto);
	   }
       }
   }
   printf ("\n\nStarting communications modules \n");
   for (loop=0; loop<comm_cnt; loop ++)
   {
	 (* comm_mods[loop]->start)(&protocols);
   }

   printf ("\n\nStarting cli  modules \n");
   for (loop=0; loop<cli_cnt; loop ++)
   {
	 (* cli_mods[loop]->start)(&protocols);
   }

   printf ("\n\nlisting switch modules \n");
   for (loop=0; loop<switch_cnt; loop ++)
   {
	   printf("Switch registered --");
	   printf ("%s %d\n", switches[loop]->name,
			   switches[loop]->type);
	   printf("in switch lib\n");
	            (* switches[loop]->print_func)();
   }
   for (loop=0; loop<proto_cnt; loop ++)
   {
	   int msg_cnt;
	   char msg[256];

       if (protocols[loop] != NULL) {
	   memset(msg, 0, 256);
	   printf("Protocol registered : ");
	   printf ("%s %d\n\n", protocols[loop]->name,
			   protocols[loop]->type);

	   printf("protocol lib :");
	            (* protocols[loop]->print_func)();
	   printf("\n\n");

#if 0
           for (msg_cnt=0;msg_cnt<protocols[loop]->msg_cnt; msg_cnt ++)
	   {
		   printf("Protocol msg id %d %d -- ", 
                      protocols[loop]->message[msg_cnt].type,
                      protocols[loop]->message[msg_cnt].message_id);
                   (*protocols[loop]->message[msg_cnt].func)(&msg);
	           printf("\n\n");
	   }
#endif
       }

   }
   while (!exit_main)
       sleep(1);

   exit_handler();
   

   return 0;

}
