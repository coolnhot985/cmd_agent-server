#ifndef _PARSE_H_
#define _PARSE_H_

#include "main.h"

typedef struct _cmd_t {
    char*   cmd_type;
    int     sequence;
    char*   miner_mac;
    char*   path;
}cmd_t;

typedef struct _agent_t {
    int     fd; 
    char*   miner_mac;
}agent_t;

json_object*    parse_string_to_json(char* str);
int             get_agent_type(json_object *json);
char*           parse_json_to_string(json_object *json_obj);
cmd_t*          parse_json_cmd(json_object *json_obj);
agent_t*        parse_json_agent(json_object *json_obj);

#endif
