#ifndef __LIB_FLOW_H__
#define __LIB_FLOW_H__


enum flw_msg_id {
	FLW_ADD_FLW,
	FLW_ADD_FLW_IF,
	FLW_DEL_FLW_IF,
	FLW_DEL_FLW,
	FLW_SET_AGE,
	FLW_SET_MAX_AGE,
	FLW_DUMP,
	FLW_MAX_MSG
};

struct flw_msg_param {
     void * dev_hdl;
     int flow_id;
     union {
	     int intf_id;
	     int age;
	     int max_age;
     };
     int dump;
};

/* Supported Tuples */

enum flow_tuple {
in_port=0,
in_phy_port,
dl_src,
dl_dst,
dl_type,
dl_vlan_pcp,
nw4_src,
nw4_dst,
nw6_src,
nw6_dst,
nw_proto,
nw_ecn,
nw_tos,
tp_src,
tp_dst,
udp_src,
udp_dst,
ipv6_label,
mpls_label,
ipv6_ext_hdr,
icmp4_code,
icmp6_code,
icmp6_tll,
icmp6_sll,
ip6_nd_target,
arp_op,
arp_spa,
arp_sha,
arp_tpa,
arp_tha,
} enum_tuple;

/* Supported actions */
enum flow_action {
fwd_queue=0,
drop,
out_port,
push_vlan_id,
pop_vlan_id,
set_vlan_id,
set_vlan_priority,
push_mpls,
pop_mpls,
set_dl_src,
set_dl_dst,
set_ip_src,
set_ip_dst,
set_tcp_src,
set_tcp_dst,
set_dscp,
set_ecn,
set_ip_ttl,
dec_ip_ttl,
set_ip_flabel,
set_extended_header,
set_mpls,
set_mpls_traffic,
set_mpls_ttl,
set_mpls_header,
dec_mpls_ttl,
copy_in_ttl,
copy_out_ttl,
set_bottom_stack,
set_icmp_code,
set_icmp_type,
set_icmp_nd,
set_nd_sll,
meter,
set_sha,
set_spa,
set_tha,
set_tpa,
set_arp_op,
};


struct tuple {
   enum flow_tuple id;
   unsigned char value[16];
};

struct action {
   enum flow_action id;
   unsigned char value[16];
};


struct flow {
	struct tuple *tuples;
	struct action *actions;
	int num_tuples;
	int num_actions;
};


/*
 * Flow add,lookup,delete
 */

typedef int (*flow_op_add)(int dev_id,int flow_table,struct flow* flow);
typedef int (*flow_op_lookup)(int dev_id,int flow_table,struct flow* flow,unsigned char *buffer);
typedef int (*flow_op_delete)(int dev_id,int flow_table,struct flow* flow);

struct flw_device {
	void *dev_hdl;
	int device_id;
	flow_op_add f_add;
	flow_op_lookup f_lookup;
	flow_op_delete f_delete;
	int active; //this is 1 if added and 0 after the device is deleted
	void * priv; //pointer to private data for each device, where the tuples and actions are stored.
        char * name;

};

void *table_flow_create(void *params, int socket_id, int entry_size);
int table_flow_free(void *table);
int table_flow_entry_add(void *table, void *key,	void *entry,int *key_found,void **entry_ptr);
int table_flow_entry_delete(void *table,void *key,int *key_found,void *entry);
int table_entry_lookup(void *table,void *key,void *entry);

#endif
