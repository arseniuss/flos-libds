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
    } ds_ ## __name ## _vec_t;                                                                                         \
                                                                                                                       \
    static inline void ds_ ## __name ## _vec_create(ds_ ## __name ## _vec_t *vector) {                                 \
        ds_vec_create((ds_vec_t *)vector);                                                                             \
    }                                                                                                                  \
                                                                                                                       \
    static inline void ds_ ## __name ## _vec_append(ds_ ## __name ## _vec_t *vector, __type value) {                   \
        ds_vec_ensure_free_space((ds_vec_t *)vector, sizeof(__type));                                                  \
        vector->data[vector->values++] = value;                                                                        \
    }                                                                                                                  \
                                                                                                                       \
    static inline void ds_ ## __name ## _vec_append_ptr(ds_ ## __name ## _vec_t *vector, __type *value) {              \
        ds_vec_ensure_free_space((ds_vec_t *)vector, sizeof(__type));                                                  \
        vector->data[vector->values++] = *value;                                                                       \
    }                                                                                                                  \
                                                                                                                       \
    static inline void ds_ ## __name ## _vec_destroy(ds_ ## __name ## _vec_t *vector) {                                \
        ds_vec_destroy((ds_vec_t *)vector);                                                                            \
    }                                                                                                                  \
                                                                                                                       \
    static inline void ds_ ## __name ## _vec_append_array(ds_ ## __name ## _vec_t *vector, __type const *data,         \
            size_t size) {                                                                                             \
        ds_vec_reserve((ds_vec_t *)vector, vector->values + size, sizeof(__type));                                     \
        memcpy(vector->data + vector->values, data, size * sizeof(__type));                                            \
        vector->values = vector->values + size;                                                                        \
    }                                                                                                                  \
                                                                                                                       \
    static inline void ds_ ## __name ## _vec_remove_noorder(ds_ ## __name ## _vec_t *vector, size_t index) {           \
        assert(index < vector->values);                                                                                \
                                                                                                                       \
        vector->data[index] = vector->data[--vector->values];                                                          \
    }                                                                                                                  \
                                                                                                                       \
    static inline void ds_ ## __name ## _vec_remove(ds_ ## __name ## _vec_t *vector, size_t index) {                   \
        assert(index < vector->values);                                                                                \
                                                                                                                       \
        vector->values--;                                                                                              \
        memcpy(&vector->data[index], &vector->data[index + 1], sizeof(__type) * (vector->values - index));             \
    }                                                                                                                  \
                                                                                                                       \
    static inline void ds_ ## __name ## _vec_sort(ds_ ## __name ## _vec_t *vector, ds_cmp_func_t cmp_func) {           \
        if (vector->values >= 2)                                                                                       \
            qsort(vector->data, vector->values, sizeof(__type), cmp_func);                                             \
    }                                                                                                                  \
                                                                                                                       \
    static inline void ds_ ## __name ## _vec_uniq(ds_ ## __name ## _vec_t *vector, ds_cmp_func_t cmp_func) {           \
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
    void *data;
    size_t values;
    size_t allocs;
    ds_alloc_func_t alloc_func;
    ds_realloc_func_t realloc_func;
    ds_free_func_t free_func;
} ds_vec_t;

void ds_vec_ensure_free_space(ds_vec_t *vector, size_t type_size);
void ds_vec_create(ds_vec_t *vector);
void ds_vec_destroy(ds_vec_t *vector);
void ds_vec_reserve(ds_vec_t *vector, size_t new_size, size_t type_size);

DS_DECL_VECTOR(int, int)

static inline int ds_int_cmp_func(const void *first, const void *second) {
    int f = *(int *) first, s = *(int *) second;

    if (f < s) return -1;
    if (f > s) return 1;
    return 0;
}

#endif /* DS__VEC_H */
