#include "parse.h"
#include "cmd_agent.h"

/** @brief      parse_string_to_json    문자열을 JSON 오브젝트로 변환
  * @params     str     변환할 문자열
  * @return     json    JSON 오브젝트
  */
json_object* parse_string_to_json(char* str) {
    json_object *json;
    enum json_type type;

    json = json_tokener_parse(str);

    return json;
}


/** @brief      get_agnet_type  제이썬 첫번째 key 만 검사하여 에이젼트 타입 체크
  * @params     json            클라이언트로부터 수신한 패킷에서 변환된 JSON
  * @return     agent_type      약속된 key:value 가 아닐 경우 잘못된 패킷
  */
agent_type_t get_agent_type(json_object *json) {
    enum json_type  type;
    int             result = -1;
    char            *str = NULL;

    json_object_object_foreach(json, key, val) {
        type = json_object_get_type(val);
        switch (type) {
            case json_type_string :
                str = json_object_get_string(val);
                if (str != NULL) {
                    if (strcmp(str, AGENT_TYPE_LINUX_NVIDIA) == 0) {
                        return REQ_LINUX_CLAYMORE;
                    } else if (strcmp(str, AGENT_TYPE_UX) == 0) {
                        return REQ_UX;
                    }
                }
        }
    }

    return REQ_INVALID;
}

char* parse_json_to_string(json_object *json_obj) {
    enum json_type type;

    json_object_object_foreach(json_obj, key, val) {
        type = json_object_get_type(val);
        switch (type) {
            case json_type_null :
                break;
            case json_type_boolean :
                break;
            case json_type_double :
                break;
            case json_type_int:
                break;
            case json_type_object:
                break;
            case json_type_array:
                break;
            case json_type_string: 
                printf("type: json_type_string, ");
                printf("value: [%s]\n", json_object_get_string(val));
                break;
        }
    }
}
