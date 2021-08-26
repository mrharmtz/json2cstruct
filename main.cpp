#include <iostream>
#include <cctype>
using namespace std;


typedef struct{

    char streetAddress[20 + 1]; //string
    char city[20 + 1]; //string
    char state[20 + 1]; //string
    char postalCode[20 + 1]; //string

}address_t;

typedef enum{

    type_home = 0,
    type_office = 1

}type_t;

typedef struct{

    type_t type;//string
    char number[12 + 1];//string

}phoneNumber_t;

typedef struct{

    char name[20 + 1];

}children_t;

typedef struct{

    char first_name[20 + 1];//string
    char last_name[20 + 1];//string
    int is_alive;//boolean
    int age;//number
    address_t address;//dictionary
    phoneNumber_t phoneNumber[2];//static array
    size_t children_len; children_t *children;//dynamic array
    char* spouse; //dynamic string
}person_t;

int Json2CStruct_static_string_t(char* dest, const char* start_stream, const char* end_stream, int n){

    return 0;
}

#define Json2CStruct_dict_start_bracket '{'
#define Json2CStruct_dict_finish_bracket '}'
#define Json2CStruct_whitespace_char_list " \n\t\r"

int Json2CStruct_is_white_space(const char* c){
    return isspace(*c);
}

int Json2CStruct_dict_limits(const char *stream_start, const char *stream_end, const char **start, const char **end){

    int push_counter = 1;

    if(stream_start == stream_end)
        return -1;

    for (; Json2CStruct_is_white_space(stream_start); ++stream_start);
    
    if(Json2CStruct_dict_start_bracket != *stream_start++)
        return -2;

    *start = stream_start;

    for(; stream_start != stream_end && push_counter > 1; ++stream_start)
    {
        if (Json2CStruct_dict_start_bracket == *stream_start)
            ++push_counter;

        if (Json2CStruct_dict_finish_bracket == *stream_start)
            --push_counter;
    }
    
    *end = stream_start - 1;

    return 0;
}

int Json2CStruct_address_t(address_t* dest, const char* start_stream, const char* end_stream){



    return 0;
}

const char* find_eos(const char* stream){

    if(!*stream) return stream;

    while(*(++stream));

    return stream;
}

int main()
{
    address_t address;
    const char *stream = "{ \"streetAddress\": \"21 2nd Street\", \"city\": \"New York\", \"state\": \"NY\", \"postalCode\": \"10021-3100\"}";
    const char *eos = find_eos(stream);
    const char *object_start, *object_end;

    cout << "completed with " << Json2CStruct_dict_limits(stream, eos, &object_start, &object_end) << endl;

    return 0;
}
