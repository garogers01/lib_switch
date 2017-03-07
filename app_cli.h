#ifndef __APP_MSG_H__

struct app_cmd {
    int id;
    char *command_name;
    char *command_options;
};

struct avail_cmds {
     int id;
     char *func_name;
     int (*cli_cmd) (char *cmd_str);
     struct app_cmd cmds[];
}; 

#endif  // __APP_MSG_H__
