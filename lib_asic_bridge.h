#ifndef __LIB_ASIC_BRIDGE_H__
#define __LIB_ASIC_BRIDGE_H__

/* Interface for a learning bridge */

#define ETHER_ALEN 6

struct mac_entry {

   unsigned char  ether_addr[ETHER_ALEN];
   unsigned int vlan_id;
};

int asic_br_add_br (int br_id); /* Creates a new bridge table */
int asic_br_del_br (int br_id); /* Deletes a bridge table */
int asic_br_add_br_if (int br_id, int intf_id); /* adds an interface to a bridge */
int asic_br_del_br_if (int br_id, int intf_id); /* delete an interface from a bridge */
int asic_br_set_age (int br_id, int age); /* Set bridge learning ageing */
int asic_br_set_max_age (int br_id, int max_age); /* Set the maximum ageing for a bridge */
int asic_br_dump (int br_id); /* dump to screen the bridge details */
int asic_br_add_mac (int br_id, struct mac_entry mac); /* Add a static mac to the bridge */

#endif
