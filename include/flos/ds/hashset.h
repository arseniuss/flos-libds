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

#ifndef DS__HASHSET_H
#define DS__HASHSET_H

#include <flos/ds/common.h>

typedef struct {
    struct ds_hashset_entry_s **data;
    int slots;
    int entries;
    ds_hash_func_t hash_func;
    ds_cmp_func_t cmp_func;
    ds_alloc_func_t alloc_func;
    ds_realloc_func_t realloc_func;
    ds_free_func_t free_func;
    ds_clean_func_t clean_func;
} ds_hashset_t;

typedef struct ds_hashset_iter_s {
    ds_hashset_t *hashset;
    int slot;
    struct ds_hashset_entry_s *entry;
} ds_hashset_iter_t;

int ds_hashset_create(ds_hashset_t *hs, size_t initial_size, ds_hash_func_t hash_func, ds_cmp_func_t cmp_func);
int ds_hashset_create_ext(ds_hashset_t *hs, size_t initial_size, ds_hash_func_t hash_func, ds_cmp_func_t cmp_func,
                          ds_alloc_func_t alloc_func, ds_realloc_func_t realloc_func, ds_free_func_t free_func,
                          ds_clean_func_t clean_func);
void ds_hashset_destroy(ds_hashset_t *hs);

int ds_hashset_reserve(ds_hashset_t *hs, int req_slots);
void *ds_hashset_insert(ds_hashset_t *hs, const void *data, size_t size);
void *ds_hashset_insert_ext(ds_hashset_t *hs, const void *data, size_t size, size_t offset);
void *ds_hashset_search(const ds_hashset_t *hs, const void *data);
void ds_hashset_remove(ds_hashset_t *hs, const void *data);
void ds_hashset_remove_direct(ds_hashset_t *hs, const void *data);

void ds_hashset_clear(ds_hashset_t *hs);

void ds_hashset_iter_reset(ds_hashset_t *hs, ds_hashset_iter_t *iter);
void *ds_hashset_iter_next(ds_hashset_iter_t *iter);

#endif /* DS__HASHSET_H */
