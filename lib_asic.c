
#include <stdio.h>
#include <signal.h>
#include "common.h"
#include "app_msgs.h"
#include "lib_bridge.h"
#include "lib_asic_bridge.h"


struct sw asic_sw;
char asic_name[]="ASIC_SW";

int asic_print()
{
	printf("%s %d\n",asic_sw.name, asic_sw.type);

   return 1;
}
struct br_device asic_br_dev={
	        0,
		asic_br_add_br ,
		asic_br_del_br ,
		asic_br_add_br_if ,
		asic_br_del_br_if ,
		asic_br_set_age ,
		asic_br_set_max_age ,
		asic_br_dump ,
};

int asic_proto_register(int proto_id, int (*func)(void *))
{

    printf("Registering Protocol %s %d\n",asic_sw.name, asic_sw.type);

    asic_br_dev.dev_hdl=(void *)&asic_sw;

    switch (proto_id) {
	    case  PROTOCOL_BRIDGE :
                 (*func) (&asic_br_dev);
		 break;

	    default :
		   printf("Asic doesn't support protocol %d\n", proto_id);
    };	    

    return 1;
}

int halt ()
{
    printf("Halt Nothing to do %s\n", asic_name);
    return 1;
}

int init (int (*swregister)(int, void *))
{
   asic_sw.name=asic_name;
   asic_sw.type=ASIC;
   asic_sw.print_func=asic_print;
   asic_sw.proto_reg_func=asic_proto_register;
   asic_sw.halt=halt;
   (*swregister) (MOD_SWITCH, &asic_sw);

   return 1;
}
