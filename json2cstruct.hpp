#ifndef _JSON_2_CSTRUCT_HPP_
#define _JSON_2_CSTRUCT_HPP_

/*
** json supports:
** number - primitieve
** string - primitieve
** boolean - primitieve
** array - container
** dictionary - container
** null - primitieve
*/

/*
**
** this library works by defining the struct you want to fill using macros dark magic,
** after you define the structure, using macros the library will define the necessary functionality
** to read a json file that matches exactly to the structure you defined,
** if the json file does not answer the criteria exactly as you defined it,
** the function defined will FAIL, and will return an error code
**
*/

#define Json2CStructDefine_static_single_StructMember(type, name, len) type name;
#define Json2CStructDefine_static_array_StructMember(type, name, len) type name[len];
#define Json2CStructDefine_dynamic_single_StructMember(type, name, len) type* name;
#define Json2CStructDefine_dynamic_array_StructMember(type, name, len) type ## _list name;

#define Json2CStructDefineStructMember(value_metadata, value_type, value_name, value_length)\
    Json2CStructDefine_ ## value_metadata ## _StructMember(value_type, value_name, value_length)

#define Json2CStructDefineStruct(struct_name, member_list)\
typedef struct{ \
        member_list(Json2CStructDefineStructMember)\
}struct_name

#define Json2CStructMemberIncrease(value_metadata, value_type, value_name, value_length) + 1

#define Json2CStructMemberCount(struct_name, member_list) (0 member_list(Json2CStructMemberIncrease))

#define Json2CStructMemberToStringListMember(value_metadata, value_type, value_name, value_length) # value_name ,

#define Json2CStructMemberStringList(struct_name, member_list) { member_list(Json2CStructMemberToStringListMember) NULL}

#define Json2CStructObjectStartBracket '{'
#define Json2CStructObjectFinishBracket '}'
#define Json2CStructObjectKeyValueDelim ':'
#define Json2CStructObjectEntryDelim ','

#define Json2CStructArrayStartBracket '['
#define Json2CStructArrayFinishBracket ']'
#define Json2CStructArrayEntryDelim ','

#define Json2CStructStringDelim '"'
#define Json2CStructStringBackSlash '\\'

#define Json2CStructNumericNegativeSign '-'
#define Json2CStructNumericPositiveSign '+'
#define Json2CStructNumericFractionStart '.'
#define Json2CStructNumericUpperExponentStart 'E'
#define Json2CStructNumericLowerExponentStart 'e'

#define Json2CStructBoolTrue "true"
#define Json2CStructBoolFalse "false"

#define Json2CStructNull "null"

int Json2CStructIsWhiteSpace(const char* c);
void Json2CStructIgnoreWhiteSpace(const char** stream_start, const char* stream_end);

//start tests
int Json2CStructIsStartString(const char* c);
int Json2CStructIsStartNumeric(const char* c);
int Json2CStructIsStartObject(const char* c);
int Json2CStructIsStartArray(const char* c);
int Json2CStructIsStartBoolean(const char* c);
int Json2CStructIsStartNull(const char* c);

//span calculators
int Json2CStructStringSpan(const char *stream_start, const char *stream_end, const char **start, const char **end);
int Json2CStructNumericSpan(const char *stream_start, const char *stream_end, const char **start, const char **end);
int Json2CStructObjectSpan(const char *stream_start, const char *stream_end, const char **start, const char **end);
int Json2CStructArraySpan(const char *stream_start, const char *stream_end, const char **start, const char **end);
int Json2CStructBooleanSpan(const char *stream_start, const char *stream_end, const char **start, const char **end);
int Json2CStructNullSpan(const char *stream_start, const char *stream_end, const char **start, const char **end);

//value parser
int Json2CStructParse_static_array_char(char* dest[], unsigned len, const char* stream_start, const char* stream_end);
int Json2CStructParse_static_single_float(float* dest, unsigned len, const char* stream_start, const char* stream_end);

#define Json2CStructDefine_static_single_PointerCast(type) (type *)
#define Json2CStructDefine_static_array_PointerCast(type) (type **)
#define Json2CStructDefine_dynamic_single_PointerCast(type) (type **)
#define Json2CStructDefine_dynamic_array_PointerCast(type) (type ## _list *)

#define Json2CStructParseMember(value_metadata, value_type, value_name, value_length) \
        if((status = Json2CStructParse_ ## value_metadata ## _ ## value_type (Json2CStructDefine_ ## value_metadata ## _PointerCast(value_type)&(dest-> ## value_name)\
                                                                                                                            , value_length\
                                                                                                                            , values_indexes[value_index].start\
                                                                                                                            , values_indexes[value_index].end)))\
            return status;\
        ++value_index;

#define Json2CStructDefineStructStaticSingleParser(struct_name, member_list) \
    int Json2CStructParse_static_single_ ## struct_name(struct_name* dest, unsigned len, const char* stream_start, const char* stream_end);

#define Json2CStructImplementStructStaticSingleParser(struct_name, member_list) \
int Json2CStructParse_static_single_ ## struct_name(struct_name* dest, unsigned len, const char* stream_start, const char* stream_end){ \
\
    int status, value_index = 0;\
    const char *object_start, *object_end;\
    _Json2CStructValueIndexInfo values_indexes[Json2CStructMemberCount(struct_name, member_list)]; \
    const char* key_list[] = Json2CStructMemberStringList(struct_name, member_list); \
\
    if((status = Json2CStructObjectSpan(stream_start, stream_end, &object_start, &object_end)))\
        return status;\
\
    memset(values_indexes, 0, sizeof(values_indexes));\
\
    if((status = _Json2CStructObjectSearchKeyValues(object_start + 1, object_end - 1, key_list, values_indexes)))\
        return status;\
\
    member_list(Json2CStructParseMember)\
\
    return 0;\
}


//internals
typedef struct{

    const char* start;
    const char* end;

}_Json2CStructValueIndexInfo;

int _Json2CStructKeysEqual(const char **stream_start, const char* stream_end, const char* key);
int _Json2CStructValueSpan(const char **stream_start, const char *stream_end, const char **end);
int _Json2CStructObjectSearchKeyValues(const char *stream_start, const char *stream_end, const char** key_list, _Json2CStructValueIndexInfo* value_index);




#endif // _JSON_2_CSTRUCT_HPP_
