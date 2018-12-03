#ifndef _PARSE_H_
#define _PARSE_H_

#include "main.h"
#include "json.h"

json_object*    parse_string_to_json(char* str);
agent_type_t    get_agent_type(json_object *json);
char*           parse_json_to_string(json_object *json_obj);

#endif

