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

#include <stdio.h>
#include <stdlib.h>

#include <ds/common.h>

void*
ds_alloc_def(void *old, size_t size) {
    if (old != NULL) {
        perror("ds_alloc_def: alloc mem free mem");
        exit(1);
    }

    return malloc(size);
}

void*
ds_realloc_def(void *old, size_t size) {
    return realloc(old, size);
}

void
ds_free_def(void *old) {
    free(old);
}

static int __is_prime(int n) {
    if (n <= 1)
        return 0;
    if (n == 2)
        return 1;
    if (n % 2 == 0)
        return 0;

    for (int i = 3; i * i <= n; i += 2)
        if (n % i == 0)
            return 0;

    return 1;
}

int
ds_next_prime(int n) {
    while (!__is_prime(n))
        n++;
    return n;
}
