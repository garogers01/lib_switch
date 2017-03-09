#include <stdio.h>
#include <signal.h>
#include <string.h>
#include "app_msgs.h"
#include "lib_flow.h"
#include "common.h"
#include <netinet/ether.h>

enum flow_cmds {
     CMD_ADD=1,
     CMD_DEL,
     CMD_DUMP
};

struct flw_device *flw_devs[256];
int flw_dev_cnt = 0;

int flw_add_flw (void * parameter)
{
	struct flw_msg_param *flw_msg = (struct flw_msg_param*)parameter;

	printf("%s %d\n", __FUNCTION__, __LINE__);

	flw_devs[flw_msg->flow_id]->f_add(flw_msg->flow_id,0,0);
	return 1;
}

int flw_del_flw (void * parameter)
{
	struct flw_msg_param *flw_msg = (struct flw_msg_param*)parameter;

	printf("%s %d\n", __FUNCTION__, __LINE__);
	flw_devs[flw_msg->flow_id]->f_delete(flw_msg->flow_id,0,0);
	return 1;
}

int flw_add_flw_if (void * parameter)
{
	struct flw_msg_param *flw_msg = (struct flw_msg_param*)parameter;

	printf("%s %d\n", __FUNCTION__, __LINE__);
#if 0
	flw_devs[flw_msg->flow_id]->flw_add_flw_if(
			flw_msg->flow_id,
			flw_msg->intf_id);
#endif
	return 1;
}

int flw_del_flw_if (void * parameter)
{
	struct flw_msg_param *flw_msg = (struct flw_msg_param*)parameter;

#if 0
	printf("%s %d\n", __FUNCTION__, __LINE__);
	flw_devs[flw_msg->flow_id]->flw_add_flw_if(
			flw_msg->flow_id,
			flw_msg->intf_id);
#endif
	return 1;
}

int flw_set_age (void * parameter)
{
	struct flw_msg_param *flw_msg = (struct flw_msg_param*)parameter;

#if 0
	printf("%s %d\n", __FUNCTION__, __LINE__);
	flw_devs[flw_msg->flow_id]->flw_add_flw_if(
			flw_msg->flow_id,
			flw_msg->age);
#endif
	return 1;
}

int flw_set_max_age (void * parameter)
{
	struct flw_msg_param *flw_msg = (struct flw_msg_param*)parameter;

#if 0
	printf("%s %d\n", __FUNCTION__, __LINE__);
	flw_devs[flw_msg->flow_id]->flw_add_flw_if(
			flw_msg->flow_id,
			flw_msg->max_age);
#endif
	return 1;
}

int flw_dump (void * parameter)
{
	struct flw_msg_param *flw_msg = (struct flw_msg_param*)parameter;

	printf("%s %d\n", __FUNCTION__, __LINE__);
	return 1;
}

/* Commands */

void cmd_help ()
{

 printf ("Flow Commands : \n \
 help -- This help message \n \
 show -- Show all available information \n \
 load -- Load rules from a file \n \
 filter -- Filter command options an actions below \n \
 Filter match \n \
 * cmd must be one of [add, del, dump] \n \
 * dev=a must be int \n \
 * tbl=a must be int \n \
 * in-port=a must be int \n \
 * in-phy-port=a must be int \n \
 * dl-src=aa:bb:cc:dd:ee:ff  must be Hex\n \
 * dl-dst=aa:bb:cc:dd:ee:ff  must be Hex\n \
 * dl-type=aabb  must be Hex\n \
 * dl-vlan=a must be int\n \
 * dl-vlan-pcp=a must be int\n \
 * nw-src=a.b.c.d must be int\n \
 * nw-dst=a.b.c.d must be int\n \
 * nw-proto=a must be int\n \
 * tp-src=a must be int\n \
 * tp-dst=a must be int\n \
\
 Filter actions \n \
\
 * drop \n \
 * out_port=n \n \
 * enqueue_port=n \n \
 * enqueue_queue=n \n \
 * normal \n \
 * flood \n \
 * all \n \
 * controller \n \
 * push_vlan=vid \n \
 * set_field \n \
 * push_mpls \n \
 * pop_mpls \n \
 * push_pbb \n \
 * pop_pbb \n \
 * mod_vlan_vid=vid \n \
 * mod_vlan_pcp=pcp \n \
 * strip_vlan \n \
 * mod_dl_src=xx:xx:xx:xx:xx:xx \n \
 * mod_dl_dst=xx:xx:xx:xx:xx:xx \n \
 * mod_nw_src=x.x.x.x \n \
 * mod_nw_dst=x.x.x.x \n \
 * mod_tp_src=x \n \
 * mod_tp_dst=x \n \
 * mod_nw_tos=x \n \
 * resubmit \n \
 * set_queue=x \n \
 * set_mpls=x \n \
 \n\n"
  ); 
}

void fib_cmd_filter(char *cmd)
{
   char *option;
   const char token[]=" ";
   char * command;
   char * val;
   int rqd_field=0;
   struct tuple tuples[25];
   struct action actions[25];
   struct flow flow;
   int tuple_cnt=0;
   int action_cnt=0;
   struct ether_addr *addr;
   char *field;
   char temp_char;
   short dev_id=0;
   short tbl_id=0;
   short flow_cmd=0;
   char  dump_buf[256];
   
   flow.tuples = &tuples[0];
   flow.actions = &actions[0];

   printf("Filter : %s\n", cmd);
   
   command = strdup(cmd); 
   while ((option = strsep(&command, token)) != NULL)   
   {
         printf ("Option : %s\n", option);

         if (strncmp(option,"filter",6)==0)
            continue;

         /* We need a command (add, del, dump) */

         if (strncmp(option,"add",3)==0)
         {
            printf("Option : add\n");
            flow_cmd=CMD_ADD;
            continue;
         }
         else if (strncmp(option,"del",3)==0)
         {
            printf("Option : del\n");
            flow_cmd=CMD_DEL;
            continue;
         }
         else if (strncmp(option,"dump",4)==0)
         {
            printf("Option : dump\n");
            flow_cmd=CMD_DUMP;
            continue;
         }

         if (flow_cmd==0)
         {
                printf("Invalid command  %s\n", option);
                goto out;
         }

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
                if ((dev_id > flw_dev_cnt) || (dev_id < 0)) {
                  printf("Device does not exist %d\n", dev_id);
                  goto out;
                }
            }
            continue; /* Dont increment tuple counter */
           
         }

         /* We need a table */
         else if (strncmp(option,"tbl",3)==0)
         {
            printf("Option : tbl id\n");
            val=strstr(option,"=");
            if (val==NULL)
            {
                printf("Invalid table id %s\n", option);
                goto out;
            }
            else
            {
                tbl_id=(short)atoi(val+1);
                printf("val %s %d\n", val+1,tbl_id);
                if ((tbl_id < 0)) {
                  printf("Invalid table id %d\n", tbl_id);
                  goto out;
                }
            }
            continue; /* Dont increment tuple counter */
           
         }

/* Ports */
  
         else if (strncmp(option,"in-phy-port",11)==0)
         {
            rqd_field = 1;
            printf("Option : in-phy-port\n");
            tuples[tuple_cnt].id=in_phy_port;
            val=strstr(option,"=");
            if (val==NULL)
            {
                printf("Invalid option %s\n", option);
                goto out;
            }
            else
            {
                *((short *)&tuples[tuple_cnt].value[0])=(short)(atoi(val+1));
                printf("val %s %d\n", val+1,*(short*)(&tuples[tuple_cnt].value[0]));
            }
         }
         else if (strncmp(option,"in-port",7)==0)
         {
            if (rqd_field != 1)
            {
               printf("Incorrect field order \n");
               goto out;
            }
            rqd_field = 2;
            printf("Option : in-port\n");
            tuples[tuple_cnt].id=in_port;
            val=strstr(option,"=");
            if (val==NULL)
            {
                printf("Invalid option %s\n", option);
                goto out;
            }
            else
            {
                *((short *)&tuples[tuple_cnt].value[0])=(short)(atoi(val+1));
                printf("val %s %d\n", val+1,*(short*)(&tuples[tuple_cnt].value[0]));
            }
         }

/* Ethernet */
         else if (strncmp(option,"dl-dst",6)==0)
         {
            if (rqd_field != 2)
            {
               printf("Incorrect field order \n");
               goto out;
            }
            rqd_field = 3;
            printf("Option : dl-dst\n");
            tuples[tuple_cnt].id=dl_dst;
            val=strstr(option,"=");
            if (val==NULL)
            {
                printf("Invalid option %s\n", option);
                goto out;
            }
            else
            {

                printf("val %s\n ", val+1);
                struct ether_addr addr2;
               
                addr = ether_aton(val+1);

                addr = ether_aton_r (val+1, &addr2); 
                if (addr == NULL)
                {
                   printf("Invalid mac\n");
                   goto out;
                }
                field = ether_ntoa(&addr2);
                printf("val %s %s\n", val+1,field);
/*
                    ether_ntoa((struct ether_addr *)&tuples[tuple_cnt].value));
*/
               //memcpy(&tuples[tuple_cnt].value[0], &addr2.ether_addr_octet[0],6);
               tuples[tuple_cnt].value[0]=addr->ether_addr_octet[0];
               tuples[tuple_cnt].value[1]=addr->ether_addr_octet[1];
               tuples[tuple_cnt].value[2]=addr->ether_addr_octet[2];
               tuples[tuple_cnt].value[3]=addr->ether_addr_octet[3];
               tuples[tuple_cnt].value[4]=addr->ether_addr_octet[4];
               tuples[tuple_cnt].value[5]=addr->ether_addr_octet[5];
            }
         }

         else if (strncmp(option,"dl-src",6)==0)
         {
            if (rqd_field != 3)
            {
               printf("Incorrect field order \n");
               goto out;
            }
            rqd_field = 4;
            printf("Option : dl-src\n");
            tuples[tuple_cnt].id=dl_src;
            val=strstr(option,"=");
            if (val==NULL)
            {
                printf("Invalid option %s\n", option);
                goto out;
            }
            else
            {
                struct ether_addr *addr;
/*
                ether_aton_r(val+1,
                   &addr);
                   (struct ether_addr *)tuples[tuple_cnt].value);
*/
                addr = ether_aton(val+1);
                printf("val %s %s\n", val+1,
                    ether_ntoa(addr));
               //memcpy(&tuples[tuple_cnt].value[0], &addr->ether_addr_octet[0],6);
               tuples[tuple_cnt].value[0]=addr->ether_addr_octet[0];
               tuples[tuple_cnt].value[1]=addr->ether_addr_octet[1];
               tuples[tuple_cnt].value[2]=addr->ether_addr_octet[2];
               tuples[tuple_cnt].value[3]=addr->ether_addr_octet[3];
               tuples[tuple_cnt].value[4]=addr->ether_addr_octet[4];
               tuples[tuple_cnt].value[5]=addr->ether_addr_octet[5];
            }
         }

         else if (strncmp(option,"dl-type",6)==0)
         {
            if (rqd_field != 4)
            {
               printf("Incorrect field order \n");
               goto out;
            }
            rqd_field = 5;
            printf("Option : dl-type\n");
            tuples[tuple_cnt].id=dl_type;
            val=strstr(option,"=");
            if (val==NULL)
            {
                printf("Invalid option %s\n", option);
                goto out;
            }
            else
            {
                *((short *)&tuples[tuple_cnt].value[0])=(short)
                    strtol(val+1,NULL, 16);
                printf("val %s %x\n", val+1,*(short*)(&tuples[tuple_cnt].value[0]));
                printf("val %x \n", (short)strtol(val+1, NULL, 16));

               /* Fix the Endianess for HW */
               //temp_char=tuples[tuple_cnt].value[0];
               //tuples[tuple_cnt].value[0]=tuples[tuple_cnt].value[1];
               //tuples[tuple_cnt].value[1]=temp_char;
            }
         }

/* IP v4 */
         else if (strncmp(option,"nw-dst",6)==0)
         {
            if (rqd_field != 5)
            {
               printf("Incorrect field order \n");
               goto out;
            }
            rqd_field = 6;
            printf("Option : nw-dst\n");
            tuples[tuple_cnt].id=nw4_dst;

            val=strstr(option,"=");
            if (val==NULL)
            {
                printf("Invalid option %s\n", option);
                goto out;
            }
            else
            {
                inet_pton(AF_INET,val+1,(void *) &tuples[tuple_cnt].value[0]);
                printf("val %s %u.%u.%u.%u\n", val+1,
                        *(unsigned char*)(&tuples[tuple_cnt].value[0]),
                        *(unsigned char*)(&tuples[tuple_cnt].value[1]),
                        *(unsigned char*)(&tuples[tuple_cnt].value[2]),
                        *(unsigned char*)(&tuples[tuple_cnt].value[3]));
               
               /* Fix the Endianess for HW */
               temp_char=tuples[tuple_cnt].value[0];
               tuples[tuple_cnt].value[0]=tuples[tuple_cnt].value[3];
               tuples[tuple_cnt].value[3]=temp_char;
               temp_char=tuples[tuple_cnt].value[1];
               tuples[tuple_cnt].value[1]=tuples[tuple_cnt].value[2];
               tuples[tuple_cnt].value[2]=temp_char;
               
            }
         }

         else if (strncmp(option,"nw-src",6)==0)
         {
            if (rqd_field != 6)
            {
               printf("Incorrect field order \n");
               goto out;
            }
            rqd_field = 7;
            printf("Option : nw-src\n");
            tuples[tuple_cnt].id=nw4_src;

            val=strstr(option,"=");
            if (val==NULL)
            {
                printf("Invalid option %s\n", option);
                goto out;
            }
            else
            {
                inet_pton(AF_INET,val+1,(void *) &tuples[tuple_cnt].value[0]);
                printf("val %s %u.%u.%u.%u\n", val+1,
                        *(unsigned char*)(&tuples[tuple_cnt].value[0]),
                        *(unsigned char*)(&tuples[tuple_cnt].value[1]),
                        *(unsigned char*)(&tuples[tuple_cnt].value[2]),
                        *(unsigned char*)(&tuples[tuple_cnt].value[3]));
               /* Fix the Endianess for HW */
               temp_char=tuples[tuple_cnt].value[0];
               tuples[tuple_cnt].value[0]=tuples[tuple_cnt].value[3];
               tuples[tuple_cnt].value[3]=temp_char;
               temp_char=tuples[tuple_cnt].value[1];
               tuples[tuple_cnt].value[1]=tuples[tuple_cnt].value[2];
               tuples[tuple_cnt].value[2]=temp_char;
            }
         }

         else if (strncmp(option,"nw-ecn",6)==0)
         {
            if (rqd_field != 7)
            {
               printf("Incorrect field order \n");
               goto out;
            }
            rqd_field = 8;
            printf("Option : nw-ecn\n");
            tuples[tuple_cnt].id=nw_ecn;
            val=strstr(option,"=");
            if (val==NULL)
            {
                printf("Invalid option %s\n", option);
                goto out;
            }
            else
            {
                *((short *)&tuples[tuple_cnt].value[0])=(unsigned short)atoi(val+1);
                printf("val %s %u\n", val+1,*(unsigned short*)(&tuples[tuple_cnt].value[0]));
            }
         }

         else if (strncmp(option,"nw-proto",8)==0)
         {
            if (rqd_field != 8)
            {
               printf("Incorrect field order \n");
               goto out;
            }
            rqd_field = 9;
            printf("Option : nw-proto\n");
            tuples[tuple_cnt].id=nw_proto;

            val=strstr(option,"=");
            if (val==NULL)
            {
                printf("Invalid option %s\n", option);
                goto out;
            }
            else
            {
                *((short *)&tuples[tuple_cnt].value[0])=(short)atoi(val+1);
                printf("val %s %d\n", val+1,*(short*)(&tuples[tuple_cnt].value[0]));
            }
         }

#if 0
         else if (strncmp(option,"nw-ecn",6)==0)
         {
            if (rqd_field != 9)
            {
               printf("Incorrect field order \n");
               goto out;
            }
            rqd_field = 10;
            printf("Option : nw-ecn\n");
            tuples[tuple_cnt].id=nw_ecn;
            val=strstr(option,"=");
            if (val==NULL)
            {
                printf("Invalid option %s\n", option);
                goto out;
            }
            else
            {
                *((short *)&tuples[tuple_cnt].value[0])=(short)ntohs(atoi(val+1));
                printf("val %s %d\n", val+1,*(short*)(&tuples[tuple_cnt].value[0]));
            }
         }
#endif
         else if (strncmp(option,"tp-dst",6)==0)
         {
            if (rqd_field != 9)
            {
               printf("Incorrect field order \n");
               goto out;
            }
            rqd_field = 11;
            printf("Option : tp-dst\n");
            tuples[tuple_cnt].id=tp_dst;
            val=strstr(option,"=");
            if (val==NULL)
            {
                printf("Invalid option %s\n", option);
                goto out;
            }
            else
            {
                *((unsigned short *)&tuples[tuple_cnt].value[0])=(unsigned short)atoi(val+1);
                printf("val %s %u\n", val+1,*(unsigned short*)(&tuples[tuple_cnt].value[0]));
               /* Fix the Endianess for HW */
               //temp_char=tuples[tuple_cnt].value[0];
               //tuples[tuple_cnt].value[0]=tuples[tuple_cnt].value[1];
               //tuples[tuple_cnt].value[1]=temp_char;
            }
         }

         else if (strncmp(option,"tp-src",6)==0)
         {
            if (rqd_field != 11)
            {
               printf("Incorrect field order \n");
               goto out;
            }
            rqd_field = 12;
            printf("Option : tp-src\n");
            tuples[tuple_cnt].id=tp_src;
            val=strstr(option,"=");
            if (val==NULL)
            {
                printf("Invalid option %s\n", option);
                goto out;
            }
            else
            {
                *((unsigned short *)&tuples[tuple_cnt].value[0])=(unsigned short)atoi(val+1);
                printf("val %s %u\n", val+1,*(unsigned short*)(&tuples[tuple_cnt].value[0]));
               /* Fix the Endianess for HW */
               //temp_char=tuples[tuple_cnt].value[0];
               //tuples[tuple_cnt].value[0]=tuples[tuple_cnt].value[1];
               //tuples[tuple_cnt].value[1]=temp_char;
            }
         }
         else if (strncmp(option,"action",6)==0)
         {
             break;
         }

         else
         {
            printf("Option : unknown option %s\n", option);
            goto out;
         }         
         tuple_cnt ++;
   }
   flow.num_tuples=tuple_cnt;
   while ((option = strsep(&command, token)) != NULL)   
   {
         printf ("Action : %s\n", option);

         if (strncmp(option,"drop",4)==0)
         {
            actions[action_cnt].id=drop;
            continue;
         }
         else if (strncmp(option,"out-port",8)==0)
         {
            actions[action_cnt].id=out_port;
            val=strstr(option,"=");
            if (val==NULL)
            {
                printf("Invalid option %s\n", option);
                goto out;
            }
            else
            {
               *((short *)&actions[action_cnt].value[0])=(short)atoi(val+1);
               printf("val %s %d\n", val+1,*(short*)(&actions[action_cnt].value[0]));
            }
         }
         else
         {
            printf("Option : unknown option %s\n", option);
            goto out;
         }         
        
      action_cnt ++;
   }

   flow.num_actions=action_cnt;

   if (flow_cmd==CMD_ADD)
   {
        /* Add flow to HW */
        flw_devs[dev_id]->f_add(dev_id, tbl_id, &flow);
   }
   else if (flow_cmd==CMD_DEL)
   {
        /* Del flow to HW */
        flw_devs[dev_id]->f_delete(dev_id, tbl_id, &flow);
   }
   else if (flow_cmd==CMD_DUMP)
   {
        /* Add flow to HW */
        flw_devs[dev_id]->f_lookup(dev_id, tbl_id, &flow, &dump_buf);
   }
    
out:
   return;

}

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
                fib_cmd_filter(buf);
             }
             fclose(fp);
        }
}

void cmd_show(char *cmd)
{
   int loop;

   printf("Existing flow devices : \n");
   for(loop=0;loop<flw_dev_cnt;loop++)
   {
      printf("Dev id %d name %s\n", loop, flw_devs[loop]->name); 
   }  
}


/* minimum protocol device  constructs */

int register_dev (void *param)
{
	printf("Register Flow device : %s %d\n", __FUNCTION__, __LINE__);
	flw_devs[flw_dev_cnt]=param;
	flw_dev_cnt ++;

	return 1;
}

struct messages flw_message[] = {
             {PROTOCOL_FLOW,FLW_ADD_FLW,flw_add_flw},
             {PROTOCOL_FLOW,FLW_DEL_FLW,flw_del_flw},
             {PROTOCOL_FLOW,FLW_ADD_FLW_IF,flw_add_flw_if},
             {PROTOCOL_FLOW,FLW_DEL_FLW_IF,flw_del_flw_if},
             {PROTOCOL_FLOW,FLW_SET_AGE,flw_set_age},
             {PROTOCOL_FLOW,FLW_DUMP,flw_dump},
             {PROTOCOL_FLOW,FLW_SET_MAX_AGE,flw_set_max_age}};



int cli_cmd (char *cmd_str)
{
   printf("Flow Protocol %s %d\n", __FUNCTION__, __LINE__);

   if (strncmp(cmd_str,"help",4)==0) {
      cmd_help();
   }
   else if (strncmp(cmd_str, "filter",6)==0){
       fib_cmd_filter(cmd_str);
   }
   else if (strncmp(cmd_str, "show",4)==0){
      cmd_show(cmd_str + 4); /* Past the show */ 
   }
   else if (strncmp(cmd_str, "load",4)==0){
      cmd_load(cmd_str + 4); /* Past the show */ 
   }
   else 
      printf("flow : unknown command %s \n", cmd_str);
}

struct protocol flow = {
	"FLOW",
	PROTOCOL_FLOW,
	2,
	FLW_MAX_MSG,
	flw_message
};

int protocol_flw_print()
{
	printf("Flow Protocol : %s %d\n",flow.name, flow.type);

   return 1;
}

int halt()
{
   printf("Halt Flow Protocol : %s %d\n",flow.name, flow.type);

   return 1;
}


int init (int (*protoregister)(int, struct protocol *proto_var))
{
    flow.register_proto = register_dev; 
    flow.print_func=protocol_flw_print;
    flow.cli_cmd=cli_cmd;
    flow.halt = halt;
    (*protoregister) (MOD_PROTOCOL, &flow);
    return 1;
}
