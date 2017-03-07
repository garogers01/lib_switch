#ifndef __LIB_ASIC_BRIDGE_H__
#define __LIB_ASIC_BRIDGE_H__

/* Interface for a flow switch */

int flow_stub_table_flow_entry_add(int dev_id,int flow_table,struct flow* flow);
int flow_stub_table_entry_lookup(int dev_id,int flow_table,struct flow* flow,unsigned char *buffer);
int flow_stub_table_flow_entry_delete(int dev_id,int flow_table,struct flow* flow);

void *flow_stub_table_flow_create(void *params, int socket_id, unsigned int entry_size);
int flow_stub_table_flow_free(void *table);

#endif
