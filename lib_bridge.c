#include <stdio.h>
#include <signal.h>
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

int cli_cmd (char *cmd_str)
{
   printf("bridge command %s \n", cmd_str);

   
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
