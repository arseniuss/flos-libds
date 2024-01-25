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

#include <assert.h>

#include <ds/common.h>
#include <ds/vec.h>

#define MAX(a, b)       ((a) > (b) ? (a) : (b))

#define GROWTH_FACTOR   3/2

void
ds_vec_ensure_free_space(ds_ptr_vec_t *vector, size_t type_size) {
    assert(vector->alloc_func);
    assert(vector->realloc_func);

    if (vector->data == NULL) {
        vector->values = 0;
        vector->allocs = 32;
        vector->data = vector->alloc_func(NULL, vector->allocs * type_size);
    } else if (vector->values == vector->allocs) {
        vector->allocs = MAX(vector->allocs + 1, vector->allocs * GROWTH_FACTOR);
        vector->data = vector->realloc_func(vector->data, vector->allocs * type_size);
    }
}

void
ds_vec_create_ext(ds_ptr_vec_t *vector, ds_alloc_func_t alloc_func, ds_realloc_func_t realloc_func,
        ds_free_func_t free_func) {
    vector->data = NULL;
    vector->values = 0;
    vector->allocs = 0;

    vector->alloc_func = alloc_func;
    vector->realloc_func = realloc_func;
    vector->free_func = free_func;
}

void
ds_vec_create(ds_ptr_vec_t *vector) {
    ds_vec_create_ext(vector, ds_alloc_def, ds_realloc_def, ds_free_def);
}

void
ds_vec_destroy(ds_ptr_vec_t *vector) {
    if (vector->data != NULL) {
        vector->free_func(vector->data);
        vector->data = NULL;
        vector->values = 0;
        vector->allocs = 0;
    }

    vector->alloc_func = NULL;
    vector->realloc_func = NULL;
    vector->free_func = NULL;
}

void
ds_vec_reserve(ds_ptr_vec_t *vector, size_t new_size, size_t type_size) {
    if (new_size > vector->allocs) {
        vector->allocs = new_size;
        vector->data = vector->realloc_func(vector->data, vector->allocs * type_size);
    }
}

int
ds_vec_search(ds_ptr_vec_t *vector, const void *value, ds_cmp_func_t cmp_func) {
    for (int i = 0; i < vector->values; i++) {
        if (cmp_func(vector->data[i], value) == 0) return i;
    }

    return -1;
}
