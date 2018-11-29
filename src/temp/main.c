#include "json.h"
#include "cmd_agent.h"

int main(void){
    const char *string = NULL;
    json_object * jobj = json_object_new_object();

    json_object *recv;

    json_object *jstring_1 = json_object_new_string("test for json_1");
    json_object *jstring_2 = json_object_new_string("test for json_2");

    json_object_object_add(jobj, "Test1", jstring_1);
    json_object_object_add(jobj, "Test2", jstring_2);

    
    printf("jobj [%s]\n", json_object_to_json_string(jobj));
    
    // json 데이터를 string 으로 방법 2
    string = json_object_get_string(jobj);
    recv = json_tokener_parse(string);

    enum json_type type;

    json_object_object_foreach(recv, key, val) {
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
            case json_type_string: printf("type: json_type_string, ");
                                   printf("value: [%s]\n", json_object_get_string(val));
                                   break;
        }
    }

    return 0;
}
