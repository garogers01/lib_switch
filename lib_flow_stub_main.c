
#include "common.h"
#include "stdio.h"
#include <signal.h>
#include "app_msgs.h"
#include "lib_flow.h"
#include "lib_flow_stub.h"


struct sw flow_stub_sw;
char flow_stub_name[]="FLOW_STUB_SW";

int flow_stub_print()
{
	printf("Name %s Type %d\n",flow_stub_sw.name, flow_stub_sw.type);

   return 1;
}
struct flw_device flow_stub_flw_dev={
	        0,
	        0,
                flow_stub_table_flow_entry_add,
                flow_stub_table_entry_lookup,
                flow_stub_table_flow_entry_delete,
	        0,
	        0
		//flow_stub_table_flow_create ,
		//flow_stub_table_flow_free 
};

int flow_stub_proto_register(int proto_id, int (*func)(void *))
{

    printf("FLow Stub Register Name %s Type %d\n",flow_stub_sw.name, flow_stub_sw.type);

    flow_stub_flw_dev.dev_hdl=&flow_stub_sw;
    flow_stub_flw_dev.name=flow_stub_name;

    switch (proto_id) {
	    case  PROTOCOL_FLOW :
                 (*func) (&flow_stub_flw_dev);
		 break;

	    default :
		   printf("Flow stub doesn't support protocol %d\n", proto_id);
    };	    

    return 1;
}

int halt ()
{
   printf("Halting %s\n", flow_stub_name);
 
   return 1;
}

int init (int (*swregister)(int, void *))
{
   flow_stub_sw.name=flow_stub_name;
   flow_stub_sw.type=ASIC;
   flow_stub_sw.print_func=flow_stub_print;
   flow_stub_sw.proto_reg_func=flow_stub_proto_register;
   (*swregister) (MOD_SWITCH, &flow_stub_sw);

   return 1;
}
