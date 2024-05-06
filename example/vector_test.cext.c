#include "../include/cext.h"
#include <stdio.h>

int main()
{
vector *v0;
v0 = new_vector(2,sizeof(int));
int value_0=5;
            if(vector_push_back(v0,sizeof(int),&value_0)){fprintf(stderr, "Error at %s:%d:%s\n", __FILE__, __LINE__, __func__);return -1;}

vector *v = new_vector(2,sizeof(int));
    for(int i=0;i<3;++i)
    {
if(vector_push_back(v,sizeof(int),&i)){fprintf(stderr, "Error at %s:%d:%s\n", __FILE__, __LINE__, __func__);return -1;}
    }
if(vector_pop_back(v)){fprintf(stderr, "Error at %s:%d:%s\n", __FILE__, __LINE__, __func__);return -1;}

int value_back_1;
 if(vector_back(v,&value_back_1)){fprintf(stderr, "Error at %s:%d:%s\n", __FILE__, __LINE__, __func__);return -1;}
int value_back_2;
 if(vector_back(v0,&value_back_2)){fprintf(stderr, "Error at %s:%d:%s\n", __FILE__, __LINE__, __func__);return -1;}
    int a=value_back_1+value_back_2;
    printf("%d\n",a);
int value_idx_4;
 if(vector_getN(v,0,&value_idx_4)){fprintf(stderr, "Error at %s:%d:%s\n", __FILE__, __LINE__, __func__);return -1;}
int value_back_3;
 if(vector_back(v0,&value_back_3)){fprintf(stderr, "Error at %s:%d:%s\n", __FILE__, __LINE__, __func__);return -1;}
    a=value_idx_4+value_back_3;
    printf("%d\n",a);
int value_idx_5;
 if(vector_getN(v,5,&value_idx_5)){fprintf(stderr, "Error at %s:%d:%s\n", __FILE__, __LINE__, __func__);return -1;}
    //a=value_idx_5; // Error
vector_free(v0);
vector_free(v);
return 0;}
