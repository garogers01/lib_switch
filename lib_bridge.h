#ifndef __LIB_BRIDGE_H__
#define __LIB_BRIDGE_H__


enum br_msg_id {
	BR_ADD_BR,
	BR_ADD_BR_IF,
	BR_DEL_BR_IF,
	BR_DEL_BR,
	BR_SET_AGE,
	BR_SET_MAX_AGE,
	BR_DUMP,
	BR_MAX_MSG
};

struct br_msg_param {
     void *dev_hdl;
     int bridge_id;
     union {
	     int intf_id;
	     int age;
	     int max_age;
     };
     int dump;
};

struct br_device {
	void * dev_hdl;
/* Minimal Bridge Functions */
	int (*br_add_br) (int br_id);
	int (*br_del_br) (int br_id);
	int (*br_add_br_if) (int br_id, int intf_id);
	int (*br_del_br_if) (int br_id, int intf_id);
	int (*br_set_age) (int br_id, int age);
	int (*br_set_max_age) (int br_id, int max_age);
	int (*br_dump) (int br_id);
        int (*br_add_mac) (int br_id, int intf_id, unsigned char mac[ETHER_ALEN]);
        int (*br_del_mac) (int br_id, int intf_id, unsigned char mac[ETHER_ALEN]);

/* vlan aware Bridge Functions 
 * return NULL if not supported
*/
        int (*br_add_vlan_mac) (int br_id, int intf_id, short vlan_id, unsigned char mac[ETHER_ALEN]);
        int (*br_del_vlan_mac) (int br_id, int intf_id, short vlan_id, unsigned char mac[ETHER_ALEN]);
        int (*br_add_vlan)(int br_id, int intf_id, short vlan_id, short tos);

};

#endif
