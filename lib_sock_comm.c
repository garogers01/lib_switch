#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/socketvar.h>
#include <sys/errno.h>
#include <netinet/in.h>
#include <net/if.h>

#include "common.h"
#include "app_msgs.h"


struct comm sock_comm;

char sock_mod_name[]="SOCK";
int ch_id=0;

/*
struct protocol *protocols[];
*/

int sock_comm_print()
{
	printf("Socket listner %s %d\n",sock_comm.name, sock_comm.id);

   return 1;
}


int sock_comm_start (struct protocol **protocols)
{

    ch_id = fork();
    if (ch_id == 0)
    {
         int sock,clntlen,client_fd,len;
         struct sockaddr_in clnt,svr;
	    /* Child */
         sock = socket(AF_INET, SOCK_STREAM, 0);
         if(sock == -1) {
	    printf("Socket Open Error : %s\n", strerror(errno));
	    return 0;
          }
	 
          /* bind server port */
          svr.sin_family = AF_INET;
          svr.sin_addr.s_addr = htonl(INADDR_ANY);
          svr.sin_port = htons(6123);

          if(bind(sock, (struct sockaddr *) &svr, sizeof(svr))<0) {
	        perror("Cannot bind Socket port ");
                return 0;
          }

          listen(sock,5);

          while ( 1 )
	  {
              int th_id;
              clntlen = sizeof(clnt);
              client_fd = accept(sock, (struct sockaddr *) &clnt, &clntlen);
              if(client_fd<0) {
                 perror("cannot accept connection ");
                 return 0;
	      }
	      th_id=fork();
	      if (th_id == 0) {
		  int clnt_fd=client_fd;
                  char msg[SZ_BUF];
                  memset(msg, 0, SZ_BUF);
                  while ( (len = recv(clnt_fd, msg, SZ_BUF, 0)) >= 0 )
                  {
                     struct app_msg_hdr *msg_hdr = (struct app_msg_hdr *) msg;
                     if (protocols[msg_hdr->type] != 0)
	             {
	                  if (protocols[msg_hdr->type]->message[msg_hdr->message_id].func)
		              len=protocols[msg_hdr->type]->message[msg_hdr->message_id].func(msg + sizeof(struct app_msg_hdr));
	             }   
		     msg_hdr->len = sizeof(struct app_msg_hdr) + len;
		     len=send(clnt_fd,msg,msg_hdr->len,0);
		     len = 0;
                 }
                 close(clnt_fd);
	      }
	  }
         close(sock);
         return 1;
    } else {
	    return 1;
    }
}

int halt ()
{
   printf("Halting %s\n",sock_mod_name);
   kill(ch_id, SIGKILL);

   return 1;
}

int init (int (*swregister)(int, void *))
{
    sock_comm.name=sock_mod_name;
    sock_comm.id = 0;
    sock_comm.start = sock_comm_start;
    sock_comm.print_func = sock_comm_print;
    sock_comm.halt = halt;

    printf("%s %d\n",sock_comm.name, sock_comm.id);

   (*swregister) (MOD_COMM, &sock_comm);

    return 1;
}
