#include "../include/cext.h"
#include <stdio.h>

int main()
{
hashmap *h = new_hashmap_size(10,sizeof(int),sizeof(int));
    
    int key=1;
    int value=2;
if(hashmap_insert(h,(void*)&key,(void*)&value))
            {
                fprintf(stderr, "Error at %s:%d:%s\n", __FILE__, __LINE__, __func__);
                return -1;
            }
    printf("%d\n",*(int*)hashmap_get(h,(void*)&key));
    printf("%d\n",hashmap_size(h));

if(hashmap_remove(h,(void*)&key))
            {
                fprintf(stderr, "Error at %s:%d:%s\n", __FILE__, __LINE__, __func__);
                return -1;
            }
    printf("%d\n",hashmap_size(h));
 
return 0;}
