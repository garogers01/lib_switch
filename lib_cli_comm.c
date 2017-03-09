#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <signal.h>

#include "common.h"
#include "app_msgs.h"
#include "app_cli.h"

int ch_id;

void cmd_help (struct protocol **cli_protos)
{
   int loop;

   printf("Help : \n");
   printf("Global Commands : help exit quit\n");
   printf("Available sub helps (call with 'sec #;help'): \n");

   for (loop=0;loop<MAX_OBJECTS;loop++)
   {
       if (cli_protos[loop] != 0)
            printf ("%d : %s help\n",loop,cli_protos[loop]->name);
   } 
}


void run_cmd(struct protocol **cli_protos)
{

    char buf[4096];
    char *sub_cmd;
    char *bufptr;
    int proto_id;
    char *subcmd;
    int x;

    while (1)
    {
        printf("cmd shell>");
	fgets(buf,4096,stdin);
        /* Remove the new line */
        buf[strlen(buf)-1]=0x0;
        bufptr = strdup(buf);
        if (strncmp(buf,"help",4)==0)
        {
            cmd_help(cli_protos);
        }
        else if (strncmp(buf,"quit",4)==0)
        {
            printf("Quit \n");
            kill(getppid(), SIGUSR1);
            kill(getpid(), SIGKILL);
        }
        else if (strncmp(buf,"exit",4)==0)
        {
            printf("Exit \n");
            kill(getppid(), SIGUSR1);
            kill(getpid(), SIGKILL);
        }
        else if (strncmp(buf,"sec",3)==0)
        {
            printf("cmd %s\n", buf);
            sub_cmd=strstr(bufptr," ");
            printf("sub cmd [%s]\n", sub_cmd);
            proto_id = atoi(sub_cmd+1);
            printf("sub %d \n", proto_id);
            if (cli_protos[proto_id] != 0)
            {
                (* cli_protos[proto_id]->cli_cmd)(sub_cmd+3);
            }
        }
        else {
           printf("Invalid command \n");
        }

        free (bufptr);
    }
}

struct comm cli_comm;

char cli_mod_name[]="CLI";

int cli_comm_print()
{
	printf("CLI starting %s %d\n",cli_comm.name, cli_comm.id);

   return 1;
}


int cli_comm_start (struct protocol **protocols)
{

    printf("\n\ncli communications start \n");
    if((ch_id=fork()) == -1 )
    { 
       printf("Error forking cli thread : error %s \n", strerror(errno));
       return 1;
    }
    else if ( ch_id == 0)
    { 
       run_cmd(protocols);
    }
    else
    {
       /* Parent will continue now */
       return ch_id ;
    }
}

int halt()
{
    printf("Halt %s %d\n",cli_comm.name, cli_comm.id);
            kill(ch_id, SIGKILL);
}

int init (int (*swregister)(int, void *))
{
    signal(SIGINT, SIG_IGN);
    cli_comm.name=cli_mod_name;
    cli_comm.id = 0;
    cli_comm.start = cli_comm_start;
    cli_comm.print_func = cli_comm_print;
    cli_comm.halt = halt;

    printf("%s %d\n",cli_comm.name, cli_comm.id);

   (*swregister) (MOD_CLI, &cli_comm);

    return 1;
}
