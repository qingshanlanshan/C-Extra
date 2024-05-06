#include "../include/cext.h"

vector *new_vector(uint16_t len, uint16_t type_size)
{
    uint16_t size = 16;
    while (size < len)
        size <<= 1;
    void *array = malloc(type_size * size);
    vector *v = (vector *)malloc(sizeof(vector));
    memset(array, 0, type_size * size);
    v->array = array;
    v->len = 0;
    v->size = size;
    v->type_size = type_size;
    return v;
}

int vector_push_back(vector *v, uint16_t type_size, void *item)
{
    if (!v)
    {
        fprintf(stderr, "Error at %s:%d:%s\n", __FILE__, __LINE__, __func__);
        return -1;
    }

    if (type_size != v->type_size)
    {
        fprintf(stderr, "Error at %s:%d:%s\n", __FILE__, __LINE__, __func__);
        return -1;
    }

    if (v->len == UINT16_MAX)
    {
        fprintf(stderr, "Error at %s:%d:%s\n", __FILE__, __LINE__, __func__);
        return -1;
    }

    if (v->size == v->len)
    {
        void *new_array = malloc(2 * v->size * v->type_size);
        memcpy(new_array, v->array, v->size * v->type_size);
        v->size <<= 1;
        free(v->array);
        v->array = new_array;
    }
    // printf("type_size=%u\n", type_size);
    memcpy(v->array + v->len * type_size, item, type_size);
    v->len++;
    return 0;
}

int vector_pop_back(vector *v)
{
    if (!v || !v->len)
    {
        fprintf(stderr, "Error at %s:%d:%s\n", __FILE__, __LINE__, __func__);
        return -1;
    }
    v->len--;
    return 0;
}

int vector_getN(vector *v, uint16_t n, void *data)
{
    if (!v || v->len <= n)
    {
        fprintf(stderr, "Error at %s:%d:%s\n", __FILE__, __LINE__, __func__);
        return -1;
    }
    memcpy(data, v->array + n * v->type_size, v->type_size);
    return 0;
}

int vector_back(vector *v, void *data)
{
    if (!v || !v->len)
    {
        fprintf(stderr, "Error at %s:%d:%s\n", __FILE__, __LINE__, __func__);
        return -1;
    }
    if (vector_getN(v, v->len - 1, data))
    {
        fprintf(stderr, "Error at %s:%d:%s\n", __FILE__, __LINE__, __func__);
        return -1;
    }
    return 0;
}

int vector_remove(vector *v, uint16_t n)
{
    if (!v && v->len <= n)
    {
        fprintf(stderr, "Error at %s:%d:%s\n", __FILE__, __LINE__, __func__);
        return -1;
    }

    for (uint16_t idx = n; n < v->len - 2; ++idx)
    {
        memcpy(v->array + idx * v->type_size, v->array + (idx + 1) * v->type_size, v->type_size);
    }
    v->len--;
    return 0;
}

int vector_debug(vector *v)
{
    printf("size=%u, len=%u, type_size=%u\n", v->size, v->len, v->type_size);
    for (uint16_t i = 0; i < v->len; ++i)
    {
        printf("%x ", *(char *)(v->array + i * v->type_size));
    }
    printf("\n");
}

int vector_free(vector *v)
{
    free(v->array);
    free(v);
    return 0;
}

void* vector_at(vector *v, uint16_t n)
{
    if (!v || v->len <= n)
    {
        fprintf(stderr, "Error at %s:%d:%s\n", __FILE__, __LINE__, __func__);
        return NULL;
    }
    return v->array + n * v->type_size;
}

int vector_len(vector *v)
{
    if (!v)
    {
        fprintf(stderr, "Error at %s:%d:%s\n", __FILE__, __LINE__, __func__);
        return -1;
    }
    return v->len;
}

int vector_size(vector *v)
{
    if (!v)
    {
        fprintf(stderr, "Error at %s:%d:%s\n", __FILE__, __LINE__, __func__);
        return -1;
    }
    return v->size;
}