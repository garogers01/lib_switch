#ifndef __COMMON_H__
#define __COMMON_H__

#define SZ_BUF 512 /* Used for message passing */
#define MAX_OBJECTS 256 

enum {
	MOD_SWITCH,
	MOD_PROTOCOL,
	MOD_COMM,
	MOD_CLI,
	MOD_MAX
} MODULE_TYPE;

enum {
    ASIC,
    FLOW_SW
} SW_TYPE;


int print_func();

struct messages {
	int type;
	int message_id;
	int (*func)(void * parameter); 
};

struct sw {
	char *name;
	int  type; 
	int (*print_func)(void) ;
	int (*proto_reg_func)(int proto_id, int (*func)(void *)) ;
        int (*halt)(void);
	void * proto_handler[];
};

struct protocol {
	char *name;
	int  id; 
	int  type; 
	int  msg_cnt;
	struct messages *message;
        int (*register_proto)(void *func);
	int (*print_func)(void);
	int (*cli_cmd)(char *);
        int (*halt)(void);
};

struct comm {
	char *name;
	int  id; 
        int (*start)(struct protocol **var);
	int (*print_func)(void);
        int (*halt)(void);
};

struct cli {
	char *name;
	int  id; 
        int (*start)(void *var);
	int (*print_func)(void);
        int (*halt)(void);
};

int register_sw (struct sw *sw_var);

int register_proto (struct protocol *proto);
int halt (void);

#define ETHER_ALEN 6

#endif

