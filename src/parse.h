#ifndef _PARSE_H_
#define _PARSE_H_

#include "cmd_agent.h"
#include "json.h"

json_object* parse_string_to_json(char* str);
char* parse_json_to_string(json_object *json_obj);



#endif

