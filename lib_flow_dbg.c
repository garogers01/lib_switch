
#include <stdio.h>
#include "common.h"
#include "lib_flow.h"

const char *tuple_name[]={
"in_port",
"in_phy_port",
"dl_src",
"dl_dst",
"dl_type",
"dl_vlan_pcp",
"nw4_src",
"nw4_dst",
"nw6_src",
"nw6_dst",
"nw_proto",
"nw_ecn",
"nw_tos",
"tp_src",
"tp_dst",
"udp_src",
"udp_dst",
"ipv6_label",
"mpls_label",
"ipv6ext_hder",
"icmp4_code",
"icmp6_code",
"icmp6_tll",
"icmp6_sll",
"ip6_nd_target",
"arp_op",
"arp_spa",
"arp_sha",
"arp_tpa",
"arp_tha",
}; 

const char *action_name[]={
"out_port",
"enqueue_port",
"enqueue_queue",
"NORMAL",
"FLOOD",
"ALL",
"CONTROLLER",
"DROP",
"push_vlan",
"set_field",
"push_mpls",
"pop_mpls",
"push_pbb",
"pop_pbb",
"Mod_vlan_vid",
"Mod_vlan_pcp",
"Strip_vlan",
"Mod_dl_src",
"Mod_dl_dst",
"Mod_nw_src",
"Mod_nw_dst",
"Mod_tp_src",
"Mod_tp_dst",
"Mod_nw_tos",
"resubmit",
"Set_queue",
"Set_mpls"
};


int dump_flow(int dev_id, int flow_table,struct flow* flow)
{
	int i;
	printf("in dump FUNCTION\n");
	printf("device_id=%d\n",dev_id);
	printf("flow_table=%d\n",flow_table);
   
        if (flow==0) {
            printf("Null FLow passed\n");
            return 0;
        }

	for(i=0; i < flow->num_tuples; i++)
        {
		printf("TUPLE ID=%d : %s  ",flow->tuples[i].id,
                    tuple_name[flow->tuples[i].id]);

                switch (flow->tuples[i].id)
                {
                   case in_port :
                   case in_phy_port:
                   case tp_src:
                   case tp_dst:
                   case dl_vlan_pcp:
                   case nw_proto:
                   case nw_tos:
                   case nw_ecn:
                       printf("%u\n", *(unsigned short*)(&flow->tuples[i].value[0]));
                       break;
                   case dl_type:
                       printf("%x\n",*(unsigned short*)(&flow->tuples[i].value[0]));
                       break;
                   case dl_src:
                   case dl_dst:
                       printf("%x:%x:%x:%x:%x:%x \n", 
                           flow->tuples[i].value[0],
                           flow->tuples[i].value[1],
                           flow->tuples[i].value[2],
                           flow->tuples[i].value[3],
                           flow->tuples[i].value[4],
                           flow->tuples[i].value[5]);
                       break;
                   case nw4_src:
                   case nw4_dst:
                       printf("%d.%d.%d.%d\n",
                           flow->tuples[i].value[0],
                           flow->tuples[i].value[1],
                           flow->tuples[i].value[2],
                           flow->tuples[i].value[3]);
                       break;
                   case icmp4_code:
                   case mpls_label:
                   //case mpls_tc:
                   default :
                      printf("0");
                }
        }
	for(i=0; i < flow->num_actions; i++)
		printf("ACTION  ID=%d : %s %d\n",flow->actions[i].id,
                    action_name[flow->actions[i].id], (int)flow->actions[i].value[0]);
	if(flow_table != 0)
		printf("flow table not NULL\n");
	return 1;
}
