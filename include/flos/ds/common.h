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

#ifndef DS__COMMON_H
#define DS__COMMON_H

#include <stddef.h>
#include <stdint.h>

typedef uint32_t ds_hash_t;

typedef ds_hash_t (*ds_hash_func_t)(const void *data);

typedef int (*ds_cmp_func_t)(const void *first, const void *second);

typedef void (*ds_clean_func_t)(void *data);
typedef void *(*ds_alloc_func_t)(void *old, size_t size);
typedef void *(*ds_realloc_func_t)(void *old, size_t size);
typedef void (*ds_free_func_t)(void *data);

ds_hash_t ds_str_hash_def(const void *data);

void *ds_alloc_def(void *old, size_t size);
void *ds_realloc_def(void *old, size_t size);
void ds_free_def(void *old);

int ds_next_prime(int n);

#endif /* DS__COMMON_H */
