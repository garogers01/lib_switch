#include "common.h"
#include "stdio.h"


struct sw flow_sw;
char flow_name[]="FLOW_SW";

int flow_print()
{
	printf("%s %d\n",flow_sw.name, flow_sw.type);

   return 1;
}

int flow_proto_register(int proto_id, int (*func)(void *))
{
    printf("%s %d\n",flow_sw.name, flow_sw.type);

    switch (proto_id) {

	    default :
		   printf("Flow doesn't support protocol %d\n", proto_id);
    };	    
    return 1;
}

int init (int (*swregister)(int, struct sw *sw_var))
{
   flow_sw.name=flow_name;
   flow_sw.type=FLOW_SW;
   flow_sw.print_func=flow_print;
   flow_sw.proto_reg_func=flow_proto_register;
   (*swregister) (MOD_SWITCH, &flow_sw);

   return 1;
}

