#include "common.h"
#include <stdio.h>
#include <dlfcn.h>
#include <dirent.h>
#include <string.h>

struct sw *switches[256];
struct protocol *protocols[256];
int switch_cnt;
int proto_cnt;
void *hndl[256];
int hndl_cnt;

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

int register_mod (int type, void *var)
{
	switch (type) {
		case MOD_PROTOCOL :
                   return register_proto (var);
	           break;
		case MOD_SWITCH :
                   return register_sw (var);
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

   /*
    * Now that we've loaded protocols and switches
    * registers the switches with the protocols 
    */
   for (proto_loop=0; proto_loop<proto_cnt; proto_loop ++)
   {
        for (loop=0;loop<switch_cnt;loop++)
	   {
	       (* switches[loop]->proto_reg_func)(
			       protocols[proto_loop]->type,
			       protocols[proto_loop]->register_proto);
	   }
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
   for (loop=0;loop<hndl_cnt;loop++)
   {
	   dlclose(hndl[loop]);
   }
   /*
	   dlclose(hndl2);
	   */

    return 1;
}
