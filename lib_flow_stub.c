
#include <stdio.h>
#include "common.h"
#include "lib_flow.h"
#include "lib_flow_dbg.h"

int dump_flow(int dev_id, int flow_table,struct flow* flow);


int flow_stub_table_flow_entry_add(int dev_id, int flow_table,struct flow* flow)
{
	int i;
	printf("in Sutb FLOW_ADD FUNCTION\n");
	printf("device_id=%d\n",dev_id);
	printf("flow_table=%d\n",flow_table);

        dump_flow(dev_id, flow_table,flow);
   
	return 1;
}

int flow_stub_table_flow_entry_delete(int dev_id, int flow_table,struct flow* flow)
{
	int i;

        if (flow==0) {
            printf("Null FLow passed\n");
            return 0;
        }
	printf("in Stub FLOW_DELETE FUNCTION\n");
	printf("device_id=%d\n",dev_id);
	printf("flow_table=%d\n",flow_table);

        dump_flow(dev_id, flow_table,flow);

	return 1;
}

int flow_stub_table_entry_lookup(int dev_id, int flow_table,struct flow* flow,unsigned char *buffer)
{
	int i;

        if (flow==0) {
            printf("Null FLow passed\n");
            return 0;
        }

	printf("in Stub FLOW_LOOKUP FUNCTION\n");
	printf("device_id=%d\n",dev_id);
	printf("flow_table=%d\n",flow_table);

        dump_flow(dev_id, flow_table,flow);

	return 1;
}

void *flow_stub_table_flow_create(void *params, int socket_id, unsigned int entry_size)
{
	printf("Function %s  LIne #%d\n", __FUNCTION__, __LINE__);
	return (void *) 0;
}

int flow_stub_table_flow_free(void *table)
{
	printf("Function %s  LIne #%d\n", __FUNCTION__, __LINE__);
}


