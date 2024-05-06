#include "../include/cext.h"

void hashmap_debug(hashmap *h)
{
    printf("hashmap size=%u, key_type_size=%u, value_type_size=%u\n", h->size, h->key_type_size, h->value_type_size);
    for (uint16_t i = 0; i < h->v->len; i++)
    {
        vector *v = vector_at(h->v, i);
        printf("hash=%u, len=%u\n", i, v->len);
        vector_debug(v);
    }
}

hashmap *new_hashmap_size(uint16_t size, uint16_t key_type_size, uint16_t value_type_size)
{
    hashmap *h = (hashmap *)malloc(sizeof(hashmap));
    h->size = 0;
    h->key_type_size = key_type_size;
    h->value_type_size = value_type_size;
    h->v = new_vector(size, sizeof(vector));
    for (uint16_t i = 0; i < size; i++)
    {
        vector *v = new_vector(1, key_type_size + value_type_size);
        vector_push_back(h->v, sizeof(vector), (void *)v);
    }
    return h;
}

hashmap *new_hashmap(uint16_t key_type_size, uint16_t value_type_size)
{
    return new_hashmap_size(16, key_type_size, value_type_size);
}

void *_hashmap_get_key(hashmap *h, uint16_t hash, uint16_t idx)
{
    vector *v = vector_at(h->v, hash);
    return vector_at(v, idx);
}

void *_hashmap_get_value(hashmap *h, uint16_t hash, uint16_t idx)
{
    vector *v = vector_at(h->v, hash);
    return (uint8_t *)vector_at(v, idx) + h->key_type_size;
}

int hashmap_free(hashmap *h)
{
    if (!h)
    {
        fprintf(stderr, "Error at %s:%d:%s\n", __FILE__, __LINE__, __func__);
        return -1;
    }

    for (uint16_t i = 0; i < h->v->len; i++)
    {
        vector *v = vector_at(h->v, i);
        vector_free(v);
    }
    vector_free(h->v);
    free(h);
    return 0;
}

uint16_t hashmap_cal_hash(hashmap *h, const void *key)
{
    uint16_t hash = 0;

    for (uint16_t i = 0; i < h->key_type_size; i++)
    {
        hash += ((uint8_t *)key)[i];
        hash %= h->v->len;
    }
    return hash;
}

int hashmap_insert(hashmap *h, const void *key, const void *value)
{
    // hashmap_debug(h);
    uint16_t hash = hashmap_cal_hash(h, key);

    vector *v = vector_at(h->v, hash);
    void *item = malloc(h->key_type_size + h->value_type_size);
    memcpy(item, key, h->key_type_size);
    memcpy((uint8_t *)item + h->key_type_size, value, h->value_type_size);
    if (vector_push_back(v, h->key_type_size + h->value_type_size, item))
    {
        fprintf(stderr, "Error at %s:%d:%s\n", __FILE__, __LINE__, __func__);
        return -1;
    }
    // hashmap_debug(h);
    free(item);
    h->size++;
    return 0;
}

int hashmap_get_idx(hashmap *h, uint16_t hash, const void *key)
{
    vector *v = vector_at(h->v, hash);
    for (uint16_t i = 0; i < v->len; i++)
    {
        void *item = vector_at(v, i);
        if (!memcmp(item, key, h->key_type_size))
        {
            return i;
        }
    }
    return -1;
}

void *_hashmap_find(hashmap *h, const void *key)
{
    uint16_t hash = hashmap_cal_hash(h, key);
    int idx = hashmap_get_idx(h, hash, key);
    if (idx == -1)
    {
        return NULL;
    }
    return _hashmap_get_key(h, hash, idx);
}

void *hashmap_get(hashmap *h, const void *key)
{
    void *addr = _hashmap_find(h, key);
    if (addr == NULL)
        return NULL;
    return (uint8_t *)addr + h->key_type_size;
}

int hashmap_remove(hashmap *h, const void *key)
{
    uint16_t hash = hashmap_cal_hash(h, key);
    int idx = hashmap_get_idx(h, hash, key);
    if (idx == -1)
    {
        return -1;
    }
    vector *v = vector_at(h->v, hash);
    vector_remove(v, idx);
    h->size--;
    return 0;
}

int hashmap_size(hashmap *h)
{
    return h->size;
}