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

#include <flos/ds/vec.h>

#include "tap.h"

int main(void) {
    plan(14);

    {
        ds_int_vec_t ivec;

        ds_int_vec_create(&ivec);

        ok(ivec.alloc_func != NULL, "alloc_func exists");
        ok(ivec.realloc_func != NULL, "realloc_func exists");
        ok(ivec.free_func != NULL, "free_func exists");

        ds_int_vec_append(&ivec, 1);

        ok(ivec.values == 1, "appended value");

        int i = 5;

        ds_int_vec_append_ptr(&ivec, &i);

        ok(ivec.data[0] == 1, "first value 1");
        ok(ivec.data[1] == 5, "second value 5");

        int iarr[11] = {4, 42, 69, 0, 1, 2, 3, 4, 90, 101, 23};

        ds_int_vec_append_array(&ivec, iarr, 11);

        ok(ivec.values == 13, "values 13");

        ds_int_vec_remove_noorder(&ivec, 1);

        ok(ivec.values == 12, "values 12");
        ok(ivec.data[1] == 23, "data[1] == 23");

        ds_int_vec_remove(&ivec, 1);

        ok(ivec.values == 11, "values 11");
        ok(ivec.data[1] == 4, "data[1] == 4");

        ds_int_vec_sort(&ivec, ds_int_cmp_func);

        for (size_t i = 0; i < ivec.values; i++) {
            diagf("ivec.data[%ld] = %i", i, ivec.data[i]);
        }

        ds_int_vec_uniq(&ivec, ds_int_cmp_func);

        for (size_t i = 0; i < ivec.values; i++) {
            diagf("ivec.data[%ld] = %i", i, ivec.data[i]);
        }

        ds_int_vec_destroy(&ivec);

        ok(ivec.data == NULL, "data empty");
        ok(ivec.allocs == 0, "allocs 0");
        ok(ivec.values == 0, "values 0");
    }

    return 0;
}
