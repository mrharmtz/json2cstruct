#include <iostream>

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

int Json2CStruct_address_t(address_t* dest, const char* start_stream, const char* end_stream){



    return 0;
}

const char* find_eos(const char* stream){

    if(!*stream) return stream;

    while(*(++stream));

    return stream - 1;
}

int main()
{
    address_t address;
    const char *stream = "{ \"streetAddress\": \"21 2nd Street\", \"city\": \"New York\", \"state\": \"NY\", \"postalCode\": \"10021-3100\"}";
    const char *eos = find_eos(stream);

    Json2CStruct_address_t(&address, stream, eos);

    return 0;
}
