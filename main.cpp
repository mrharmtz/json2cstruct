#include <iostream>

#include "json2cstruct.hpp"

using std::cout;
using std::endl;

#define address_member_list(foreach_member)\
                            foreach_member(static_array, char, streetAddress, 20 + 1)\
                            foreach_member(static_array, char, city, 20 + 1)\
                            foreach_member(static_array, char, state, 20 + 1)\
                            foreach_member(static_array, char, postalCode, 20 + 1)

Json2CStructDefineStruct(address_t, address_member_list);

Json2CStructImplementStructStaticSingleParser(address_t, address_member_list)

#define child_member_list(foreach)\
        foreach(static_array, char, name, 20 + 1)\
        foreach(static_single, float, age, 0)

Json2CStructDefineStruct(child_t, child_member_list);

typedef enum{

    type_home = 0,
    type_office = 1

}type_t;

typedef struct{

    type_t type;//string
    char number[12 + 1];//string

}phoneNumber_t;

typedef struct{

    char first_name[20 + 1];//string
    char last_name[20 + 1];//string
    int is_alive;//boolean
    int age;//number
    address_t address;//dictionary
    phoneNumber_t phoneNumber[2];//static array
    size_t children_len; child_t *children;//dynamic array
    char* spouse; //dynamic string
}person_t;

const char* find_eos(const char* stream){

    if(!*stream) return stream;

    while(*(++stream));

    return stream;
}

Json2CStructImplementStructStaticSingleParser(child_t, child_member_list)

int main()
{
    address_t address;
    child_t child;
    const char *address_stream = "{ \"streetAddress\": \"21 2nd Street\", \"city\": \"New York\", \"state\": \"NY\" ,\"postalCode\": \"10021-3100\"}";
    const char *child_stream = "{\"name\": \"josh\", \"age\": 165.0e-1}";
    const char *full_stream =  "{\"firstName\": \"John\",  \"lastName\": \"Smith\",  \"isAlive\": true,  \"age\": 27,  \"address\": {    \"streetAddress\": \"21 2nd Street\",    \"city\": \"New York\",    \"state\": \"NY\",    \"postalCode\": \"10021-3100\"  },  \"phoneNumbers\": [    {      \"type\": \"home\",      \"number\": \"212 555-1234\"    },    {      \"type\": \"office\",      \"number\": \"646 555-4567\"    }  ],  \"children\": [],  \"spouse\": null}";
    const char *eos = 0;
    const char *object_start = 0, *object_end = 0;
    int result;

    eos = find_eos(address_stream);
    result = Json2CStructParse_static_single_address_t(&address, 0, address_stream, eos);
    cout << "Json2CStructParse_static_single_address_t completed with code " << result << endl;

    cout << "address.city = " << address.city << ", address.postalCode = " << address.postalCode << ", address.state = " << address.state << ", address.streetAddress = " << address.streetAddress << endl;

    eos = find_eos(child_stream);
    result = Json2CStructParse_static_single_child_t(&child, 0, child_stream, eos);
    cout << "Json2CStructParse_static_single_child_t completed with code " << result << endl;

    cout << "child.name = " << child.name << ", child.age = " << child.age << endl;

    return 0;
}
