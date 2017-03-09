#include <stdio.h>
#include <signal.h>
#include <string.h>
#include "common.h"
#include "app_msgs.h"
#include "app_cli.h"
#include "lib_bridge.h"

struct br_device *br_devs[MAX_OBJECTS];
int br_dev_cnt = 0;

int br_add_br (void * parameter)
{
	struct br_msg_param *br_msg = (struct br_msg_param*)parameter;

	printf("%s %d\n", __FUNCTION__, __LINE__);

	br_devs[br_msg->bridge_id]->br_add_br(br_msg->bridge_id);
	return 1;
}

int br_del_br (void * parameter)
{
	struct br_msg_param *br_msg = (struct br_msg_param*)parameter;

	printf("%s %d\n", __FUNCTION__, __LINE__);
	br_devs[br_msg->bridge_id]->br_del_br(br_msg->bridge_id);
	return 1;
}

int br_add_br_if (void * parameter)
{
	struct br_msg_param *br_msg = (struct br_msg_param*)parameter;

	printf("%s %d\n", __FUNCTION__, __LINE__);
	br_devs[br_msg->bridge_id]->br_add_br_if(
			br_msg->bridge_id,
			br_msg->intf_id);
	return 1;
}

int br_del_br_if (void * parameter)
{
	struct br_msg_param *br_msg = (struct br_msg_param*)parameter;

	printf("%s %d\n", __FUNCTION__, __LINE__);
	br_devs[br_msg->bridge_id]->br_add_br_if(
			br_msg->bridge_id,
			br_msg->intf_id);
	return 1;
}

int br_set_age (void * parameter)
{
	struct br_msg_param *br_msg = (struct br_msg_param*)parameter;

	printf("%s %d\n", __FUNCTION__, __LINE__);
	br_devs[br_msg->bridge_id]->br_add_br_if(
			br_msg->bridge_id,
			br_msg->age);
	return 1;
}

int br_set_max_age (void * parameter)
{
	struct br_msg_param *br_msg = (struct br_msg_param*)parameter;

	printf("%s %d\n", __FUNCTION__, __LINE__);
	br_devs[br_msg->bridge_id]->br_add_br_if(
			br_msg->bridge_id,
			br_msg->max_age);
	return 1;
}

int br_dump (void * parameter)
{
	struct br_msg_param *br_msg = (struct br_msg_param*)parameter;

	printf("%s %d\n", __FUNCTION__, __LINE__);
	return 1;
}

int register_dev (void *param)
{
	printf("Registering a bridge %s %d\n", __FUNCTION__, __LINE__);
	br_devs[br_dev_cnt]=param;
	br_dev_cnt ++;

	return 1;
}

struct messages br_message[] = {
             {PROTOCOL_BRIDGE,BR_ADD_BR,br_add_br},
             {PROTOCOL_BRIDGE,BR_DEL_BR,br_del_br},
             {PROTOCOL_BRIDGE,BR_ADD_BR_IF,br_add_br_if},
             {PROTOCOL_BRIDGE,BR_DEL_BR_IF,br_del_br_if},
             {PROTOCOL_BRIDGE,BR_SET_AGE,br_set_age},
             {PROTOCOL_BRIDGE,BR_DUMP,br_dump},
             {PROTOCOL_BRIDGE,BR_SET_MAX_AGE,br_set_max_age}};

void cmd_help ()
{
    printf("Bridge Commands : \n \
    help -- This help message \n \
    show -- Dump Bridge info \n \
    load -- Load bridge config from file \n \
    add-br dev=x br-id=x  -- Add a bridge br_id is integer \n \
    del-br dev=x br-id=x  -- Delete a bridge br_id is integer \n \
    add-br-if dev=x br-id=x if-id=x -- Add interface to a bridge br_id int_id \n \
    del-br-if dev=x br-id=x if-id=x -- Delete interface to a bridge br_id int_id \n \
    set-age -- dev=x br-id=x age=x  Set aging timer \n"
    );

}

void cmd_show(char *cmd)
{
   int loop;

   printf("Existing bridge devices : \n");
   for(loop=0;loop<br_dev_cnt;loop++)
   {
      printf("Dev id %d name %s\n", loop); 
   }  
}

void cmd_add_br(char *cmd)
{
   char *option;
   const char token[]=" ";
   char * command;
   char * val;
   int dev_id;
   int br_id;

   printf("Command add_br : %s\n", cmd);

   command = strdup(cmd); 
   while ((option = strsep(&command, token)) != NULL)   
   {
         printf ("Option : %s\n", option);

         if (strncmp(option,"add-br",9)==0)
            continue;

         /* We need a device first */
         if (strncmp(option,"dev",3)==0)
         {
            printf("Option : dev\n");
            val=strstr(option,"=");
            if (val==NULL)
            {
                printf("Invalid device id %s\n", option);
                goto out;
            }
            else
            {
                dev_id=(short)(atoi(val+1));
                printf("val %s %d\n", val+1,dev_id);
                if ((dev_id > br_dev_cnt-1) || (dev_id < 0)) {
                  printf("Device does not exist %d\n", dev_id);
                  goto out;
                }
            }
            continue; /* Dont increment tuple counter */
           
         }
         else if (strncmp(option,"br-id",5)==0)
         {
            printf("Option : br-id\n");
            val=strstr(option,"=");
            if (val==NULL)
            {
                printf("Invalid bridge id %s\n", option);
                goto out;
            }
            else
            {
                br_id=(short)(atoi(val+1));
                printf("val %s %d\n", val+1,br_id);
            }
            continue; /* Dont increment tuple counter */
           
         }
         else
         {
            printf("Option : unknown option %s\n", option);
            goto out;
         }         

   }
   /* Add bridge in HW */
   br_devs[dev_id]->br_add_br(br_id);
out :
   return;
}

void cmd_del_br(char *cmd)
{
   char *option;
   const char token[]=" ";
   char * command;
   char * val;
   int dev_id;
   int br_id;

   printf("Command del_br : %s\n", cmd);

   command = strdup(cmd); 
   while ((option = strsep(&command, token)) != NULL)   
   {
         printf ("Option : %s\n", option);

         if (strncmp(option,"del-br",9)==0)
            continue;

         /* We need a device first */
         if (strncmp(option,"dev",3)==0)
         {
            printf("Option : dev\n");
            val=strstr(option,"=");
            if (val==NULL)
            {
                printf("Invalid device id %s\n", option);
                goto out;
            }
            else
            {
                dev_id=(short)(atoi(val+1));
                printf("val %s %d\n", val+1,dev_id);
                if ((dev_id > br_dev_cnt-1) || (dev_id < 0)) {
                  printf("Device does not exist %d\n", dev_id);
                  goto out;
                }
            }
            continue; /* Dont increment tuple counter */
           
         }
         else if (strncmp(option,"br-id",5)==0)
         {
            printf("Option : br-id\n");
            val=strstr(option,"=");
            if (val==NULL)
            {
                printf("Invalid bridge id %s\n", option);
                goto out;
            }
            else
            {
                br_id=(short)(atoi(val+1));
                printf("val %s %d\n", val+1,br_id);
            }
            continue; /* Dont increment tuple counter */
           
         }
         else
         {
            printf("Option : unknown option %s\n", option);
            goto out;
         }         

   }
   /* Add bridge in HW */
   br_devs[dev_id]->br_del_br(br_id);
out :
   return;
}
void cmd_del_br_if(char *cmd)
{
   char *option;
   const char token[]=" ";
   char * command;
   char * val;
   int dev_id;
   int br_id;
   int if_id;

   printf("Command del_br_if : %s\n", cmd);

   command = strdup(cmd); 
   while ((option = strsep(&command, token)) != NULL)   
   {
         printf ("Option : %s\n", option);

         if (strncmp(option,"del-br-if",9)==0)
            continue;

         /* We need a device first */
         if (strncmp(option,"dev",3)==0)
         {
            printf("Option : dev\n");
            val=strstr(option,"=");
            if (val==NULL)
            {
                printf("Invalid device id %s\n", option);
                goto out;
            }
            else
            {
                dev_id=(short)(atoi(val+1));
                printf("val %s %d\n", val+1,dev_id);
                if ((dev_id > br_dev_cnt-1) || (dev_id < 0)) {
                  printf("Device does not exist %d\n", dev_id);
                  goto out;
                }
            }
            continue; /* Dont increment tuple counter */
           
         }
         else if (strncmp(option,"br-id",5)==0)
         {
            printf("Option : br-id\n");
            val=strstr(option,"=");
            if (val==NULL)
            {
                printf("Invalid bridge id %s\n", option);
                goto out;
            }
            else
            {
                br_id=(short)(atoi(val+1));
                printf("val %s %d\n", val+1,br_id);
            }
            continue; /* Dont increment tuple counter */
           
         }
         else if (strncmp(option,"if-id",5)==0)
         {
            printf("Option : if-id\n");
            val=strstr(option,"=");
            if (val==NULL)
            {
                printf("Invalid interface id %s\n", option);
                goto out;
            }
            else
            {
                if_id=(short)(atoi(val+1));
                printf("val %s %d\n", val+1,if_id);
            }
            continue; /* Dont increment tuple counter */
         }
         else
         {
            printf("Option : unknown option %s\n", option);
            goto out;
         }         

   }
   /* Add interface to bridge in HW */
   br_devs[dev_id]->br_del_br_if(br_id, if_id);
out :
   return;
}
void cmd_add_br_if(char *cmd)
{
   char *option;
   const char token[]=" ";
   char * command;
   char * val;
   int dev_id;
   int br_id;
   int if_id;

   printf("Command add-br-if : %s\n", cmd);

   command = strdup(cmd); 
   while ((option = strsep(&command, token)) != NULL)   
   {
         printf ("Option : %s\n", option);

         if (strncmp(option,"add-br-if",9)==0)
            continue;

         /* We need a device first */
         if (strncmp(option,"dev",3)==0)
         {
            printf("Option : dev\n");
            val=strstr(option,"=");
            if (val==NULL)
            {
                printf("Invalid device id %s\n", option);
                goto out;
            }
            else
            {
                dev_id=(short)(atoi(val+1));
                printf("val %s %d\n", val+1,dev_id);
                if ((dev_id > br_dev_cnt-1) || (dev_id < 0)) {
                  printf("Device does not exist %d\n", dev_id);
                  goto out;
                }
            }
            continue; /* Dont increment tuple counter */
           
         }
         else if (strncmp(option,"br-id",5)==0)
         {
            printf("Option : br-id\n");
            val=strstr(option,"=");
            if (val==NULL)
            {
                printf("Invalid bridge id %s\n", option);
                goto out;
            }
            else
            {
                br_id=(short)(atoi(val+1));
                printf("val %s %d\n", val+1,br_id);
            }
            continue; /* Dont increment tuple counter */
           
         }
         else if (strncmp(option,"if-id",5)==0)
         {
            printf("Option : if-id\n");
            val=strstr(option,"=");
            if (val==NULL)
            {
                printf("Invalid interface id %s\n", option);
                goto out;
            }
            else
            {
                if_id=(short)(atoi(val+1));
                printf("val %s %d\n", val+1,if_id);
            }
            continue; /* Dont increment tuple counter */
         }
         else
         {
            printf("Option : unknown option %s\n", option);
            goto out;
         }         

   }
   /* Add interface to bridge in HW */
   br_devs[dev_id]->br_add_br_if(br_id, if_id);
out :
    return;
}

void cmd_set_age(char *cmd)
{
   char *option;
   const char token[]=" ";
   char * command;
   char * val;
   int dev_id;
   int br_id;
   int age;

   printf("Command set_age : %s\n", cmd);

   command = strdup(cmd); 
   while ((option = strsep(&command, token)) != NULL)   
   {
         printf ("Option : %s\n", option);

         if (strncmp(option,"set-age",7)==0)
            continue;

         /* We need a device first */
         if (strncmp(option,"dev",3)==0)
         {
            printf("Option : dev\n");
            val=strstr(option,"=");
            if (val==NULL)
            {
                printf("Invalid device id %s\n", option);
                goto out;
            }
            else
            {
                dev_id=(short)(atoi(val+1));
                printf("val %s %d\n", val+1,dev_id);
                if ((dev_id > br_dev_cnt-1) || (dev_id < 0)) {
                  printf("Device does not exist %d\n", dev_id);
                  goto out;
                }
            }
            continue; /* Dont increment tuple counter */
           
         }
         else if (strncmp(option,"br-id",5)==0)
         {
            printf("Option : br-id\n");
            val=strstr(option,"=");
            if (val==NULL)
            {
                printf("Invalid bridge id %s\n", option);
                goto out;
            }
            else
            {
                br_id=(short)(atoi(val+1));
                printf("val %s %d\n", val+1,br_id);
            }
            continue; /* Dont increment tuple counter */
           
         }
         else if (strncmp(option,"age",3)==0)
         {
            printf("Option : age\n");
            val=strstr(option,"=");
            if (val==NULL)
            {
                printf("Invalid age %s\n", option);
                goto out;
            }
            else
            {
                age=(short)(atoi(val+1));
                printf("val %s %d\n", val+1,age);
            }
            continue; /* Dont increment tuple counter */
         }
         else
         {
            printf("Option : unknown option %s\n", option);
            goto out;
         }         

   }
   /* Add bridge in HW */
   br_devs[dev_id]->br_set_age(br_id, age);
out :
   return;
}
int cli_cmd (char *cmd_str);

void cmd_load(char *cmd)
{
        FILE *fp;
        char buf[4096];
        char *bufptr; 

        printf("Load Command :%s:\n", cmd);

        if (strlen(cmd) > 4) 
        {
             /* Open FIB rule file and pasre it */
             if (cmd == '\0') {
                printf("No FIB rule is found\n");
                return;
             }
             fp = fopen(cmd+1, "r");
             if (fp == NULL) {
                printf("Unable to open FIB rule %s\n", cmd+1);
                return;
             }
             while (fgets(buf, 4096, fp)!= NULL) {
                cli_cmd(buf);
             }
             fclose(fp);
        }
}

int cli_cmd (char *cmd_str)
{
   printf("bridge command %s \n", cmd_str);

   if (strncmp(cmd_str,"help",4)==0) {
      cmd_help();
   }
   else if (strncmp(cmd_str, "add-br-if",9)==0){
       cmd_add_br_if(cmd_str);
   }
   else if (strncmp(cmd_str, "del-br-if",9)==0){
       cmd_del_br_if(cmd_str);
   }
   else if (strncmp(cmd_str, "add-br",6)==0){
       cmd_add_br(cmd_str);
   }
   else if (strncmp(cmd_str, "del-br",6)==0){
       cmd_del_br(cmd_str);
   }
   else if (strncmp(cmd_str, "show",4)==0){
      cmd_show(cmd_str + 4); /* Past the show */ 
   }
   else if (strncmp(cmd_str, "set-age",7)==0){
       cmd_set_age(cmd_str);
   }
   else if (strncmp(cmd_str, "load",4)==0){
      cmd_load(cmd_str + 4); /* Past the show */ 
   }
   else 
      printf("flow : unknown command %s \n", cmd_str);

   
}

struct protocol bridge = {
	"BRIDGING",
	PROTOCOL_BRIDGE,
	1,
	BR_MAX_MSG,
	br_message
};

int protocol_br_print()
{
	printf("Print %s %d\n",bridge.name, bridge.type);

   return 1;
}

int halt()
{
   printf("Halt %s %d\n",bridge.name, bridge.type);

   return 1;
}

int init (int (*protoregister)(int, struct protocol *proto_var))
{
    bridge.register_proto = register_dev; 
    bridge.print_func=protocol_br_print;
    bridge.cli_cmd=cli_cmd;
    bridge.halt=halt;
    (*protoregister) (MOD_PROTOCOL, &bridge);
    return 1;
}
