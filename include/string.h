#ifndef CEXT_STRING_H
#define CEXT_STRING_H
#include "vector.h"

typedef struct string
{
    vector *v;
} string;

string *new_string();
int string_set(string *s, const char *str);
int string_len(string *s);
int string_size(string *s);
int string_push_back(string *s, const char *c);
int string_pop_back(string *s);
int string_compare(string *s1, const char *s2);
char string_at(string *s, const uint16_t index);
int string_append(string *s, const char *str);
int string_free(string *s);
char* string_get(string *s);

#endif