#include "../include/cext.h"

string *new_string()
{
    string *s = (string *)malloc(sizeof(string));
    s->v = new_vector(1, sizeof(char));
    return s;
}

int string_set(string *s, const char *str)
{
    if (!s)
    {
        fprintf(stderr, "Error at %s:%d:%s\n", __FILE__, __LINE__, __func__);
        return -1;
    }

    if (!str)
    {
        fprintf(stderr, "Error at %s:%d:%s\n", __FILE__, __LINE__, __func__);
        return -1;
    }

    vector_free(s->v);
    s->v = new_vector(1, sizeof(char));
    for (uint16_t i = 0; str[i]; i++)
    {
        if (vector_push_back(s->v, sizeof(char), (void *)&str[i]))
        {
            fprintf(stderr, "Error at %s:%d:%s\n", __FILE__, __LINE__, __func__);
            return -1;
        }
    }
    return 0;
}

char* string_get(string *s)
{
    if (!s)
    {
        fprintf(stderr, "Error at %s:%d:%s\n", __FILE__, __LINE__, __func__);
        return NULL;
    }

    char *str = (char *)malloc(s->v->len + 1);
    for (uint16_t i = 0; i < s->v->len; i++)
    {
        str[i] = *(char *)vector_at(s->v, i);
    }
    str[s->v->len] = '\0';
    return str;
}

int string_len(string *s)
{
    if (!s)
    {
        fprintf(stderr, "Error at %s:%d:%s\n", __FILE__, __LINE__, __func__);
        return -1;
    }
    return s->v->len;
}

int string_push_back(string *s, const char *c)
{
    if (!s)
    {
        fprintf(stderr, "Error at %s:%d:%s\n", __FILE__, __LINE__, __func__);
        return -1;
    }

    if (vector_push_back(s->v, sizeof(char), (void *)c))
    {
        fprintf(stderr, "Error at %s:%d:%s\n", __FILE__, __LINE__, __func__);
        return -1;
    }
    return 0;
}

int string_pop_back(string *s)
{
    if (!s)
    {
        fprintf(stderr, "Error at %s:%d:%s\n", __FILE__, __LINE__, __func__);
        return -1;
    }

    if (vector_pop_back(s->v))
    {
        fprintf(stderr, "Error at %s:%d:%s\n", __FILE__, __LINE__, __func__);
        return -1;
    }
    return 0;
}

char string_at(string *s, const uint16_t index)
{
    if (!s)
    {
        fprintf(stderr, "Error at %s:%d:%s\n", __FILE__, __LINE__, __func__);
        return -1;
    }

    if (index >= s->v->len)
    {
        fprintf(stderr, "Error at %s:%d:%s\n", __FILE__, __LINE__, __func__);
        return -1;
    }

    return *(char *)vector_at(s->v, index);
}

int string_append(string *s, const char *str)
{
    if (!s)
    {
        fprintf(stderr, "Error at %s:%d:%s\n", __FILE__, __LINE__, __func__);
        return -1;
    }

    if (!str)
    {
        fprintf(stderr, "Error at %s:%d:%s\n", __FILE__, __LINE__, __func__);
        return -1;
    }

    for (uint16_t i = 0; str[i]; i++)
    {
        if (vector_push_back(s->v, sizeof(char), (void *)&str[i]))
        {
            fprintf(stderr, "Error at %s:%d:%s\n", __FILE__, __LINE__, __func__);
            return -1;
        }
    }
    return 0;
}

int string_compare(string *s1, const char *s2)
{
    if (!s1 || !s2)
    {
        fprintf(stderr, "Error at %s:%d:%s\n", __FILE__, __LINE__, __func__);
        return -1;
    }

    int len_diff = 0;
    if (s1->v->len != strlen(s2))
        len_diff = s1->v->len - strlen(s2) > 0 ? 1 : -1;
    for (uint16_t i = 0; i < s1->v->len && s2[i]; i++)
    {
        char c1 = *(char *)vector_at(s1->v, i);
        char c2 = s2[i];
        if (c1 != c2)
        {
            return c1 - c2 > 0 ? 1 : -1;
            break;
        }
    }
    return len_diff;
}

int string_free(string *s)
{
    if (!s)
    {
        fprintf(stderr, "Error at %s:%d:%s\n", __FILE__, __LINE__, __func__);
        return -1;
    }

    vector_free(s->v);
    free(s);
    return 0;
}

int string_size(string *s)
{
    if (!s)
    {
        fprintf(stderr, "Error at %s:%d:%s\n", __FILE__, __LINE__, __func__);
        return -1;
    }
    return vector_size(s->v);
}