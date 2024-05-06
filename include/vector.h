#ifndef CEXT_VECTOR_H
#define CEXT_VECTOR_H
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct vector{
    uint16_t size;
    uint16_t len;
    uint16_t type_size;
    void *array;
}vector;

vector *new_vector(uint16_t len, uint16_t type_size);
int vector_push_back(vector *v, uint16_t type_size, void *item);
int vector_pop_back(vector *v);
int vector_getN(vector *v, uint16_t n, void *data);
int vector_back(vector *v, void *data);
int vector_remove(vector *v, uint16_t n);
int vector_debug(vector *v);
int vector_free(vector *v);
void* vector_at(vector *v, uint16_t n);
int vector_len(vector *v);
int vector_size(vector *v);

#endif