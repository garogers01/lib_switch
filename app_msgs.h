#ifndef __APP_MSGS_H__
#define __APP_MSGS_H__

#define PROTOCOL_APP 0
#define PROTOCOL_BRIDGE 1
#define PROTOCOL_FLOW 2

struct app_msg_hdr {
	int type;
	int message_id;
	int len;
};

enum {
	MSG_QRY_SW,
	MSG_QRY_PROTO,
	MSG_QRY_MAX
};

struct app_qry_msg {
	int qry_id;
};	

#endif

