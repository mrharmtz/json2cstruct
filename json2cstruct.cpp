#include "json2cstruct.hpp"

#include <cctype>
#include <cstring>
#include <cstdlib>

// checks if c points to a white space charecter
#define _JSON_2_CSTRUCT_IS_START_
#ifdef _JSON_2_CSTRUCT_IS_START_ //region

int Json2CStructIsWhiteSpace(const char* c){
    return isspace(*c);
}

void Json2CStructIgnoreWhiteSpace(const char** stream_start, const char* stream_end){
    for (; Json2CStructIsWhiteSpace(*stream_start) && *stream_start <= stream_end; ++*stream_start);
}

int Json2CStructIsStartNumeric(const char* c){

    return (isdigit(*c) || Json2CStructNumericNegativeSign == *c);
}

int Json2CStructIsStartString(const char* c){

    return (Json2CStructStringDelim == *c);
}

int Json2CStructIsStartObject(const char* c){

    return (Json2CStructObjectStartBracket == *c);
}

int Json2CStructIsStartArray(const char* c){

    return (Json2CStructArrayStartBracket == *c);
}

int Json2CStructIsStartBoolean(const char* c){

    return (Json2CStructBoolTrue[0] == *c || Json2CStructBoolFalse[0] == *c);
}

int Json2CStructIsStartNull(const char* c){

    return (Json2CStructNull[0] == *c);
}

#endif // _JSON_2_CSTRUCT_IS_START_

int Json2CStructStringSpan(const char *stream_start, const char *stream_end, const char **start, const char **end){

    if(stream_start > stream_end)
        return -1; //empty stream

    Json2CStructIgnoreWhiteSpace(&stream_start, stream_end);

    if(!Json2CStructIsStartString(stream_start))
        return -2; //no string starting here

    if(start)
        *start = stream_start;

    ++stream_start;

    for(; stream_start <= stream_end && Json2CStructStringDelim != *stream_start; ++stream_start)
    {
        //if escape character (\) is used ignore next character
        if(Json2CStructStringBackSlash == *stream_start){
            ++stream_start;
            continue;
        }
    }

    if(stream_start > stream_end && Json2CStructStringDelim != *stream_start)
        return -3;// reached end without closing string delimiter

    if(end)
        *end = stream_start;

    return 0;
}

//TODO: add test to check that the first digit of the numeric value is not 0, for example the values -05 or 05 are illegal
int Json2CStructNumericSpan(const char *stream_start, const char *stream_end, const char **start, const char **end){

    //json numeric grammer is constructed out of three parts: integer, fraction, and exponent
    //when both fraction and exponent can be empty strings

    if(stream_start > stream_end)
        return -1; //empty stream

    Json2CStructIgnoreWhiteSpace(&stream_start, stream_end);

    if(!Json2CStructIsStartNumeric(stream_start))
        return -2; //no string starting here

    if(start)
        *start = stream_start;

    ++stream_start;

    //scan integer portion
    for(; stream_start <= stream_end && isdigit(*stream_start); ++stream_start);

    if(Json2CStructNumericFractionStart == *stream_start){//check for fraction part
        ++stream_start;
        //scan fraction portion
        for(; stream_start <= stream_end && isdigit(*stream_start); ++stream_start);
    }

    if(Json2CStructNumericUpperExponentStart == *stream_start ||Json2CStructNumericLowerExponentStart == *stream_start){//check for exponent part
        ++stream_start;

        if(Json2CStructNumericNegativeSign == *stream_start || Json2CStructNumericPositiveSign == *stream_start)//check for sign
            ++stream_start;

        for(; stream_start <= stream_end && isdigit(*stream_start); ++stream_start);
    }

    if(end)
        *end = stream_start - 1;

    return 0;
}

// finds the object span, json can contain multiple different, sometimes nested objects.
int Json2CStructObjectSpan(const char *stream_start, const char *stream_end, const char **start, const char **end){

    int push_counter = 1, string_flag = 0;

    if(stream_start > stream_end)
        return -1; //empty stream

    //ignore white space
    Json2CStructIgnoreWhiteSpace(&stream_start, stream_end);

    if(!Json2CStructIsStartObject(stream_start))
        return -2; //no object starting here

    if(start)
        *start = stream_start;

    ++stream_start;

    for(; stream_start <= stream_end && push_counter > 0; ++stream_start)
    {
        //if inside string, and escape character (\) is used ignore next character
        if(string_flag && Json2CStructStringBackSlash == *stream_start){
            ++stream_start;
            continue;
        }

        //if detect start string, raise string flag
        if(Json2CStructStringDelim == *stream_start)
            string_flag = !string_flag;

        //ignore all characters inside string
        if(string_flag)
            continue;

        if (Json2CStructObjectStartBracket == *stream_start)
            ++push_counter;

        if (Json2CStructObjectFinishBracket == *stream_start)
            --push_counter;
    }

    if(stream_start > stream_end && push_counter > 0)
        return -3;// reached end without closing bracket

    if(end)
        *end = stream_start - 1;

    return 0;
}

int Json2CStructArraySpan(const char *stream_start, const char *stream_end, const char **start, const char **end){

    int push_counter = 1, string_flag = 0;

    if(stream_start > stream_end)
        return -1; //empty stream

    //ignore white space
    Json2CStructIgnoreWhiteSpace(&stream_start, stream_end);

    if(!Json2CStructIsStartArray(stream_start))
        return -2; //no array starting here

    if(start)
        *start = stream_start;

    ++stream_start;

    for(; stream_start <= stream_end && push_counter > 0; ++stream_start)
    {
        //if inside string, and escape character (\) is used ignore next character
        if(string_flag && Json2CStructStringBackSlash == *stream_start){
            ++stream_start;
            continue;
        }

        //if detect start string, raise string flag
        if(Json2CStructStringDelim == *stream_start)
            string_flag = !string_flag;

        //ignore all characters inside string
        if(string_flag)
            continue;

        if (Json2CStructArrayStartBracket == *stream_start)
            ++push_counter;

        if (Json2CStructArrayFinishBracket == *stream_start)
            --push_counter;
    }

    if(stream_start > stream_end && push_counter > 0)
        return -3;// reached end without closing bracket

    if(end)
        *end = stream_start - 1;

    return 0;

}

int Json2CStructBooleanSpan(const char *stream_start, const char *stream_end, const char **start, const char **end){

    if(stream_start > stream_end)
        return -1; //empty stream

    //ignore white space
    Json2CStructIgnoreWhiteSpace(&stream_start, stream_end);

    if(!Json2CStructIsStartBoolean(stream_start))
        return -2; //no boolean starting here

    if(start)
        *start = stream_start;

    if(Json2CStructBoolTrue[0] == *stream_start){
        if((long long unsigned)(stream_end - stream_start) < (sizeof(Json2CStructBoolTrue) - 2))
            return -3;

        if(end)
            *end = stream_start + (sizeof(Json2CStructBoolTrue) - 2);
    }

    if(Json2CStructBoolFalse[0] == *stream_start){

        if((long long unsigned)(stream_end - stream_start) < (sizeof(Json2CStructBoolFalse) - 2))
            return -3;

        if(end)
            *end = stream_start + (sizeof(Json2CStructBoolFalse) - 2);
    }

    return 0;
}

int Json2CStructNullSpan(const char *stream_start, const char *stream_end, const char **start, const char **end){

    if(stream_start > stream_end)
        return -1; //empty stream

    //ignore white space
    Json2CStructIgnoreWhiteSpace(&stream_start, stream_end);

    if(!Json2CStructIsStartNull(stream_start))
        return -2; //no boolean starting here

    if(start)
        *start = stream_start;

    if(Json2CStructNull[0] == *stream_start){
        if((long long unsigned)(stream_end - stream_start) < (sizeof(Json2CStructNull) - 2))
            return -3;

        if(end)
            *end = stream_start + (sizeof(Json2CStructNull) - 2);
    }

    return 0;
}

//value parser
int Json2CStructParse_static_array_char(char* dest[], unsigned len, const char* stream_start, const char* stream_end){

    size_t i;

    if(stream_start > stream_end)
        return -1; //empty stream

    Json2CStructIgnoreWhiteSpace(&stream_start, stream_end);

    if(!Json2CStructIsStartString(stream_start++))
        return -2;

    if(Json2CStructStringDelim != *stream_end)
        return -3;

    if(stream_end - stream_start + 1 > len)
        return -4;

    for (i = 0; stream_start != stream_end; ++i, ++stream_start){
        ((char*)dest)[i] = *stream_start;
    }
    
    ((char*)dest)[i] = '\0';

    return 0;
}

int Json2CStructParse_static_single_float(float* dest, unsigned len, const char* stream_start, const char* stream_end){

    char* last_pos;

    if(stream_start > stream_end)
        return -1; //empty stream

    Json2CStructIgnoreWhiteSpace(&stream_start, stream_end);

    if(!Json2CStructIsStartNumeric(stream_start))
        return -2;

    *dest = strtod(stream_start, &last_pos);

    if (last_pos - 1 != stream_end)
        return -3;
    
    return 0;
}

//internals
int _Json2CStructKeysEqual(const char **stream_start, const char* stream_end, const char* key){

    for(; *key && *stream_start <= stream_end; ++*stream_start, ++key){
        if(*key != **stream_start)
            return 0;
    }

    if((!*key && **stream_start != Json2CStructStringDelim) // if reached end of key but not end of stream
       || (*stream_start > stream_end && *key)) // if reached end of stream but not of key
        return 0;
    ++*stream_start;
    return 1;
}

int _Json2CStructValueSpan(const char **stream_start, const char *stream_end, const char **end){

    Json2CStructIgnoreWhiteSpace(stream_start, stream_end);

    do{
        if(Json2CStructIsStartString(*stream_start)){//value is json string
            Json2CStructStringSpan(*stream_start, stream_end, NULL, end);
            break;
        }

        if(Json2CStructIsStartNumeric(*stream_start)){//value is json number
            Json2CStructNumericSpan(*stream_start, stream_end, NULL, end);
            break;
        }

        if(Json2CStructIsStartObject(*stream_start)){//value is json object
            Json2CStructObjectSpan(*stream_start, stream_end, NULL, end);
            break;
        }

        if(Json2CStructIsStartArray(*stream_start)){//value is json array
            Json2CStructArraySpan(*stream_start, stream_end, NULL, end);
            break;
        }

        if(Json2CStructIsStartBoolean(*stream_start)){//value is json boolean
            Json2CStructBooleanSpan(*stream_start, stream_end, NULL, end);
            break;
        }

        if(Json2CStructIsStartNull(*stream_start)){//value is a null
            Json2CStructNullSpan(*stream_start, stream_end, NULL, end);
            break;
        }

        return -1;//unknown value type
    }while(0);

    //advance stream pointer to end of value span
    *stream_start = *end + 1;

    return 0;
}

int _Json2CStructObjectSearchKeyValues(const char *stream_start, const char *stream_end, const char** key_list, _Json2CStructValueIndexInfo* value_index){

    const char* key_start;
    int i = 0;

    for(;stream_start < stream_end; ++stream_start){

        if(Json2CStructIsWhiteSpace(stream_start))//if white space we ignore
            continue;

        if(Json2CStructIsStartString(stream_start)){//start of key in stream

            key_start = ++stream_start;

            for(i = 0 ; key_list[i] != NULL ; ++i){

                if(_Json2CStructKeysEqual(&stream_start, stream_end, key_list[i]))
                    break;

                stream_start = key_start;
            }

            if(!key_list[i])//if reached end of key list
                return -2;//key was not found in key list

            //if there a value already stored inside value_index at index i
            //duplicate value, not allowed
            if(value_index[i].start)
                return -3;

            //ignore white space
            Json2CStructIgnoreWhiteSpace(&stream_start, stream_end);

            if(Json2CStructObjectKeyValueDelim != *stream_start)
                return -4;//expected key value delimiter

            //store where value of member i starts
            value_index[i].start = ++stream_start;

            //find where value of member i ends, and store it
            if(_Json2CStructValueSpan(&stream_start, stream_end, &(value_index[i].end)))
                return -5;

            //ignore white space
            Json2CStructIgnoreWhiteSpace(&stream_start, stream_end);

            if(Json2CStructObjectFinishBracket == *stream_start)
                break;

            if(Json2CStructObjectEntryDelim != *stream_start)
                return -6; //missing entry delim

        }else{
            return -1;//error expected string key
        }
    }

    //check if all fields were found
    for(i = 0 ; key_list[i] != NULL ; ++i)
        if(!value_index[i].start)
            return -7;

    return 0;
}