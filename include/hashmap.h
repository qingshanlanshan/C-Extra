#ifndef CEXT_HASHMAP_H
#define CEXT_HASHMAP_H
#include "vector.h"

typedef struct hashmap
{
    uint16_t size;
    uint16_t key_type_size;
    uint16_t value_type_size;
    vector *v;
} hashmap;
hashmap *new_hashmap_size(uint16_t size, uint16_t key_type_size, uint16_t value_type_size);
hashmap *new_hashmap(uint16_t key_type_size, uint16_t value_type_size);
int hashmap_free(hashmap *h);
uint16_t hashmap_cal_hash(hashmap *h, const void *key);
int hashmap_insert(hashmap *h, const void *key, const void *value);
int hashmap_get_idx(hashmap *h, uint16_t hash, const void *key);
void *hashmap_get(hashmap *h, const void *key);
int hashmap_remove(hashmap *h, const void *key);
int hashmap_size(hashmap *h);


#endif