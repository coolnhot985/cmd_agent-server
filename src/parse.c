#include "parse.h"

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

