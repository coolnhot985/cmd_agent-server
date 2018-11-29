#include "tcp.h"

char* tcp_hello(void) {
    const char *str = NULL;
    char *buff = NULL;

    json_object *json = json_object_new_object();
    json_object *agent_type = json_object_new_string(AGENT_TYPE_LINUX_NVIDIA);
    
    json_object_object_add(json, "agent_type", agent_type);

    str = json_object_get_string(json);

    buff = strndup(str, strlen(str));

    //buff = (char*)malloc(SERVER_HELLO_PACKET_SIZE);

    return buff;
}

