#include "main.h"

/** @brief      parse_string_to_json    문자열을 JSON 오브젝트로 변환
  * @params     str     변환할 문자열
  * @return     json    JSON 오브젝트
  */
json_object* parse_string_to_json(char* str) {
    json_object *json;

    json = json_tokener_parse(str);

    return json;
}

/** @brief      get_agnet_type  첫번째 key 만 검사하여 에이젼트 타입 체크
  * @params     json            클라이언트로부터 수신한 패킷에서 변환된 JSON
  * @return     agent_type      약속된 key:value 가 아닐 경우 잘못된 패킷
  */
int get_agent_type(json_object *json) {
    enum json_type  type;
    const char      *value = NULL;

    json_object_object_foreach(json, key, val) {
        type = json_object_get_type(val);
        switch (type) {
            case json_type_string :
                value = json_object_get_string(val);
                if (value != NULL) {
                    if (strcmp(value, AGENT_TYPE_LINUX_NVIDIA) == 0) {
                        return REQ_LINUX_CLAYMORE;
                    } else if (strcmp(value, AGENT_TYPE_UX) == 0) {
                        return REQ_UX;
                    }
                }
                /* JSON 데이터에서 첫번째 키만 파싱 */
                goto INVALID_PACKET;
            case json_type_null:
                break;
            case json_type_boolean:
                break;
            case json_type_double:
                break;
            case json_type_int:
                break;
            case json_type_object:
                break;
            case json_type_array:
                break;
        }
    }
INVALID_PACKET:

    if (key == NULL) {
        /* 해당 조건문은 아무것도 수행하지않음
           JSON 라이브러리 KEY warning 삭제용도 */
    }

    return REQ_INVALID;
}

void parse_json_cmd(json_object *json_obj, session_t *session) {
    enum    json_type type;

    json_object_object_foreach(json_obj, key, val) {
        type = json_object_get_type(val);
        switch (type) {
            case json_type_string: 
                if (strcmp(key, "commend_type") == 0) {
                    session->cmd_type = (char*)json_object_get_string(val);
                } else if(strcmp(key, "sequence") == 0) {
                    session->sequence = atoi(json_object_get_string(val));
                } else if (strcmp(key, "miner_mac") == 0) {
                    session->miner_mac = (char*)json_object_get_string(val);
                } else if (strcmp(key, "path") == 0) {
                    if (0 != json_object_get_string_len(val)) {
                        session->path = (char*)json_object_get_string(val);
                    } else {
                        session->path = (char*)malloc(sizeof(1));
                        session->path = "\0";
                    }
                } else if (strcmp(key, "path_install") == 0) {
                    if (0 != json_object_get_string_len(val)) {
                        session->path_install = (char*)json_object_get_string(val);
                    } else {
                        session->path_install = (char*)malloc(sizeof(1));
                        session->path_install = NULL;
                    }
                }
                break;
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
        }
    }
    return;
}

void parse_json_agent(json_object *json_obj, session_t *session) {
    enum json_type  type;

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
                if (strcmp(key, "miner_mac") == 0) {
                    session->miner_mac = (char*)json_object_get_string(val);
                }
                break;
        }
    }
    return;
}

int parse_cmd_type(const char *cmd_type) {
    DEBUG("cmd_type [%s]", cmd_type);
    if (strcmp(cmd_type, "file-download") == 0) {
        return 1;
    } 

    return -1;
}

