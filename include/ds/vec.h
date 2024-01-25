/*
 * Copyright (C) 2023 Armands Arseniuss Skolmeisters <arseniuss@arseniuss.id.lv>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef DS__VEC_H
#define DS__VEC_H

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include <ds/common.h>

#define DS_DECL_VECTOR(__name, __type)                                                                                 \
    typedef struct {                                                                                                   \
        __type *data;                                                                                                  \
        size_t values;                                                                                                 \
        size_t allocs;                                                                                                 \
        ds_alloc_func_t alloc_func;                                                                                    \
        ds_realloc_func_t realloc_func;                                                                                \
        ds_free_func_t free_func;                                                                                      \
    } __name ## _vec_t;                                                                                                \
                                                                                                                       \
    typedef void (*__name ## _free_func_t)(__type data);                                                               \
    typedef char *(*__name ## _dup_func_t)(__type *target, __type const *source);                                       \
                                                                                                                       \
    static inline void __name ## _vec_create(__name ## _vec_t *vector) {                                               \
        ds_vec_create((ds_ptr_vec_t *)vector);                                                                         \
    }                                                                                                                  \
                                                                                                                       \
    static inline void __name ## _vec_append(__name ## _vec_t *vector, __type value) {                                 \
        ds_vec_ensure_free_space((ds_ptr_vec_t *)vector, sizeof(__type));                                              \
        vector->data[vector->values++] = value;                                                                        \
    }                                                                                                                  \
                                                                                                                       \
    static inline void __name ## _vec_append_ptr(__name ## _vec_t *vector, __type *value) {                            \
        ds_vec_ensure_free_space((ds_ptr_vec_t *)vector, sizeof(__type));                                              \
        vector->data[vector->values++] = *value;                                                                       \
    }                                                                                                                  \
                                                                                                                       \
    static inline void __name ## _vec_insert(__name ## _vec_t *vector, int index, __type value) {                      \
        ds_vec_ensure_free_space((ds_ptr_vec_t *)vector, sizeof(__type));                                              \
        if(vector->values - index > 0)                                                                                 \
            memmove(vector->data + index + 1, vector->data + index, (vector->values - index) * sizeof(__type));        \
        vector->values++;                                                                                              \
        vector->data[index] = value;                                                                                   \
    }                                                                                                                  \
                                                                                                                       \
    static inline void __name ## _vec_clear(__name ## _vec_t *vector) {                                                \
        vector->values = 0;                                                                                            \
    }                                                                                                                  \
                                                                                                                       \
    static inline void __name ## _vec_clear_ext(__name ## _vec_t *vector, __name ## _free_func_t free_func) {          \
        if (vector->values != 0) {                                                                                     \
            for (size_t i = 0; i < vector->values; i++) { free_func(vector->data[i]); vector->data[i] = 0; }           \
            vector->values = 0;                                                                                        \
        }                                                                                                              \
    }                                                                                                                  \
                                                                                                                       \
    static inline void __name ## _vec_destroy(__name ## _vec_t *vector) {                                              \
        ds_vec_destroy((ds_ptr_vec_t *)vector);                                                                        \
    }                                                                                                                  \
                                                                                                                       \
    static inline void __name ## _vec_append_array(__name ## _vec_t *vector, __type const *data,                       \
            size_t size) {                                                                                             \
        ds_vec_reserve((ds_ptr_vec_t *)vector, vector->values + size, sizeof(__type));                                 \
        memcpy(&vector->data[vector->values], data, size * sizeof(__type));                                            \
        vector->values = vector->values + size;                                                                        \
    }                                                                                                                  \
                                                                                                                       \
    static inline char *__name ## _vec_dup_array(__name ## _vec_t *vector, __type const *data,                         \
            size_t size, __name ## _dup_func_t dup_func) {                                                             \
        char *err = NULL;                                                                                              \
                                                                                                                       \
        ds_vec_reserve((ds_ptr_vec_t *)vector, vector->values + size, sizeof(__type));                                 \
        for (size_t i = 0; i < size; i++) {                                                                            \
            vector->data[i + vector->values] = 0;                                                                      \
            if ((err = dup_func(&vector->data[i + vector->values], &data[i]))) return err;                             \
        }                                                                                                              \
        vector->values = vector->values + size;                                                                        \
                                                                                                                       \
        return NULL;                                                                                                   \
    }                                                                                                                  \
                                                                                                                       \
    static inline void __name ## _vec_remove_noorder(__name ## _vec_t *vector, size_t index) {                         \
        assert(index < vector->values);                                                                                \
                                                                                                                       \
        vector->data[index] = vector->data[--vector->values];                                                          \
    }                                                                                                                  \
                                                                                                                       \
    static inline void __name ## _vec_remove(__name ## _vec_t *vector, size_t index) {                                 \
        assert(index < vector->values);                                                                                \
                                                                                                                       \
        vector->values--;                                                                                              \
        memcpy(&vector->data[index], &vector->data[index + 1], sizeof(__type) * (vector->values - index));             \
    }                                                                                                                  \
                                                                                                                       \
    static inline void __name ## _vec_sort(__name ## _vec_t *vector, ds_cmp_func_t cmp_func) {                         \
        if (vector->values >= 2)                                                                                       \
            qsort(vector->data, vector->values, sizeof(__type), cmp_func);                                             \
    }                                                                                                                  \
                                                                                                                       \
    static inline void __name ## _vec_uniq(__name ## _vec_t *vector, ds_cmp_func_t cmp_func) {                         \
        if (vector->values >= 2) {                                                                                     \
            int j = 1;                                                                                                 \
                                                                                                                       \
            for(size_t i = 1; i < vector->values; i++)                                                                 \
                if (cmp_func(&vector->data[i - 1], &vector->data[i]) != 0)                                             \
                    vector->data[j++] = vector->data[i];                                                               \
                                                                                                                       \
            vector->values = j;                                                                                        \
        }                                                                                                              \
    }

typedef struct {
    void **data;
    size_t values;
    size_t allocs;
    ds_alloc_func_t alloc_func;
    ds_realloc_func_t realloc_func;
    ds_free_func_t free_func;
} ds_ptr_vec_t;

void ds_vec_ensure_free_space(ds_ptr_vec_t *vector, size_t type_size);
void ds_vec_create(ds_ptr_vec_t *vector);
void ds_vec_destroy(ds_ptr_vec_t *vector);
void ds_vec_reserve(ds_ptr_vec_t *vector, size_t new_size, size_t type_size);
int ds_vec_search(ds_ptr_vec_t *vector, const void *value, ds_cmp_func_t cmp_func);

DS_DECL_VECTOR(ds_int, int)

static inline int ds_int_cmp_func(const void *first, const void *second) {
    int f = *(int *) first, s = *(int *) second;

    if (f < s) return -1;
    if (f > s) return 1;
    return 0;
}

DS_DECL_VECTOR(ds_const_str, const char *)
DS_DECL_VECTOR(ds_str, char *)

static inline int ds_str_cmp_func(const void *first, const void *second) {
    const char *f = (const char *) first, *s = (const char *) second;

    return strcmp(f, s);
}

#endif /* DS__VEC_H */
