#include "../include/cext.h"
#include <stdio.h>

int main()
{

string *str1 = new_string();
string_set(str1,"");
string *str2 = new_string();
string_set(str2,"123456789");
string *s = new_string();
string_set(s,"123");

printf("%d\n",string_len(str1));
printf("%d\n",string_len(str2));

string_set(str1,"Hello, World!");
printf("%d\n",string_compare(str1,string_get(str2)));

if(string_pop_back(str2)){fprintf(stderr, "Error at %s:%d:%s\n", __FILE__, __LINE__, __func__);return -1;}
if(string_push_back(str2,"0")){
                fprintf(stderr, "Error at %s:%d:%s\n", __FILE__, __LINE__, __func__);
                return -1;
                }
printf("%s\n",string_get(str2));

printf("%c\n",string_at(str2,0));

string_append(str1,string_get(str2));
printf("%s\n",string_get(str1));   
return 0;}
