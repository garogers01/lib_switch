#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/socketvar.h>
#include <sys/errno.h>
#include <netinet/in.h>
#include <net/if.h>
//#include <netinet/in_var.h>
#include "common.h"
#include "app_msgs.h"
#include "app_msgs.h"
#include "lib_bridge.h"


#define SZ_BUF 512

int send_main_app_msg(int sock)
{
	int loop;
	int len;
	char msg_buf[128];
	char reply_buf[128];
	struct app_msg_hdr *msg;
	struct app_qry_msg *qry_msg;
	struct br_msg_param *br_msg;

	msg = (struct app_msg_hdr *)msg_buf;
	qry_msg = (struct app_qry_msg *)(msg_buf + sizeof(struct app_msg_hdr));
	br_msg = (struct br_msg_param *)(msg_buf + sizeof(struct app_msg_hdr));

	for (loop=0; loop<MSG_QRY_MAX; loop++)
	{
		printf("send msg\n");
		msg->type=PROTOCOL_APP;
		msg->message_id=loop;
		msg->len = sizeof(struct app_msg_hdr) + sizeof(struct app_qry_msg);
		printf("len %d\n", msg->len);
		qry_msg->qry_id = 256;

		if (send(sock, msg_buf, msg->len,0) <= 0)
			printf("error sending message");
		/* wait for reply */
		while ((len=recv(sock, reply_buf, 128,0)) == 0);
		printf("len %d\n", len);
	}
	/* Bridge Protocol */
	for (loop=0; loop<BR_MAX_MSG; loop++)
	{
		printf("send msg\n");
		msg->type=PROTOCOL_BRIDGE;
		msg->message_id=loop;
		msg->len = sizeof(struct app_msg_hdr) + sizeof(struct br_msg_param);
		printf("len %d\n", msg->len);
		br_msg->dev_hdl = 256;
		br_msg->bridge_id = 0;
		br_msg->intf_id = loop;
		br_msg->dump = 0;

		if (send(sock, msg_buf, msg->len,0) <= 0)
			printf("error sending message");
		/* wait for reply */
		while ((len=recv(sock, reply_buf, 128,0)) == 0);
		printf("len %d\n", len);
	}
}

int init () {

    int sock,clntlen,clnt_fd,len;
    struct sockaddr_in clnt,svr;
    char msg[SZ_BUF];

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock == -1) {
	    printf("Socket Open Error : %s\n", strerror(errno));
	    return 0;
     }
	 
     /* bind server port */
     svr.sin_family = AF_INET;
     svr.sin_addr.s_addr = htonl(INADDR_ANY);
     svr.sin_port = htons(6123);

     if(connect(sock, (struct sockaddr *) &svr, sizeof(svr))<0) {
	   perror("Cannot open Socket port ");
           return 0;
     }
     send_main_app_msg(sock);

     close(sock);
     return 1;
}
