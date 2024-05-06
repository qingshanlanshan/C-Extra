#include "../include/cext.h"
#include <stdio.h>

int main()
{
hashmap *h = new_hashmap_size(10,sizeof(string),sizeof(string));
string *str1 = new_string();
string_set(str1,"hello");
string *str2 = new_string();
string_set(str2,"world");

if(hashmap_insert(h,str1,str2))
            {
                fprintf(stderr, "Error at %s:%d:%s\n", __FILE__, __LINE__, __func__);
                return -1;
            }

    printf("%s\n",hashmap_get(h,string_get(str1)));

if(hashmap_remove(h,str1))
            {
                fprintf(stderr, "Error at %s:%d:%s\n", __FILE__, __LINE__, __func__);
                return -1;
            }
    printf("%d\n",hashmap_size(h));
 
string_free(str1);
string_free(str2);
return 0;}
