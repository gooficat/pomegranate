#pragma once

#include <stdint.h>
#include <stdlib.h>

#define class(name)                                                                                                    \
    typedef struct name name;                                                                                          \
    struct name

#define enum(name, ...) typedef enum name __VA_ARGS__ name;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

class(str_view)
{
    char *ptr;
    u8 len;
};

#define vec_type(type)                                                                                                 \
    class(vector_##type)                                                                                               \
    {                                                                                                                  \
        type *data;                                                                                                    \
        size_t len;                                                                                                    \
        size_t cap;                                                                                                    \
    };

#define push(vec, ...)                                                                                                 \
    if (++vec.len > vec.cap)                                                                                           \
    {                                                                                                                  \
        vec.cap *= 2;                                                                                                  \
        vec.data = realloc(vec.data, vec.cap * sizeof(vec.data[0]));                                                   \
    }                                                                                                                  \
    vec.data[vec.len - 1] = __VA_ARGS__;

#define pop(vec)                                                                                                       \
    if (--vec.len > vec.cap / 2)                                                                                       \
    {                                                                                                                  \
        vec.cap /= 2;                                                                                                  \
        vec.data = realloc(vec.data, vec.cap * sizeof(vec.data[0]));                                                   \
    }

#define new(t, ...)                                                                                                    \
    ({                                                                                                                 \
        t *p = malloc(sizeof(t));                                                                                      \
        *p = (t)__VA_ARGS__;                                                                                           \
        p;                                                                                                             \
    })

#define del(v)                                                                                                         \
    if (v)                                                                                                             \
        free(v);

#define copy_str(s)                                                                                                    \
    ({                                                                                                                 \
        size_t l = strlen(s) + 1;                                                                                      \
        char *v = malloc(l);                                                                                           \
        memcpy(v, s, l);                                                                                               \
        v;                                                                                                             \
    })

#define make_vec(t) (vector_##t){.data = malloc(sizeof(t)), .len = 0, .cap = 1}