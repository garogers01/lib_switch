#include "common.h"
#include <stdio.h>
#include <dlfcn.h>
#include <dirent.h>
#include <string.h>
#include "app_msgs.h"


struct sw *switches[MAX_OBJECTS] = {};
struct protocol *protocols[MAX_OBJECTS] = {};
struct comm *comm_mods[MAX_OBJECTS] = {};
int switch_cnt=0;
int proto_cnt=0;
int comm_cnt=0;

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
    protocols[proto_var->id]=proto_var;
    proto_cnt ++;

    return 1;
}

int register_comm (struct comm *comm_var)
{
    comm_mods[comm_cnt]=comm_var;
    comm_cnt ++;

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
		              printf ("unable to open %s\n", ent->d_name);
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

	printf("%s %d\n", __FUNCTION__, __LINE__);
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
	printf("%s %d\n", __FUNCTION__, __LINE__);
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
	printf("%s %d\n",app_main.name, app_main.type);

   return 1;
}

int app_init ()
{
    app_main.register_proto = register_dev; 
    app_main.print_func=protocol_app_print;
    register_mod (MOD_PROTOCOL, &app_main);
    return 1;
}


int main ()
{
   void *hndl1;
   void *hndl2;
   int loop;
   int proto_loop;
   int (*reg_sw)(int (*register_sw)(int, void *));
   

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
   for (loop=0; loop<comm_cnt; loop ++)
   {
	 (* comm_mods[loop]->start)(&protocols);
   }

   for (loop=0; loop<switch_cnt; loop ++)
   {
	   printf("registered\n");
	   printf ("%s %d\n", switches[loop]->name,
			   switches[loop]->type);
	   printf("in lib\n");
	            (* switches[loop]->print_func)();
   }
   for (loop=0; loop<proto_cnt; loop ++)
   {
	   int msg_cnt;
	   char msg[256];

       if (protocols[loop] != NULL) {
	   memset(msg, 0, 256);
	   printf("registered\n");
	   printf ("%s %d\n", protocols[loop]->name,
			   protocols[loop]->type);
	   printf("in lib\n");
	            (* protocols[loop]->print_func)();
           for (msg_cnt=0;msg_cnt<protocols[loop]->msg_cnt; msg_cnt ++)
	   {
		   printf("id %d %d -- ", 
                      protocols[loop]->message[msg_cnt].type,
                      protocols[loop]->message[msg_cnt].message_id);
                   (*protocols[loop]->message[msg_cnt].func)(&msg);
	   }
       }

   }
   while (1)
	   sleep();
   for (loop=0;loop<hndl_cnt;loop++)
   {
	   dlclose(hndl[loop]);
   }

    return 1;
}
