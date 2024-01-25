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

#ifndef DS__PAIR_H
#define DS__PAIR_H

#include <flos/ds/vec.h>

#define DS_DECL_PAIR(__name, __type1, __type2)                                                                         \
    typedef struct {                                                                                                   \
        __type1 first;                                                                                                 \
        __type2 second;                                                                                                \
    } __name##_pair_t;

DS_DECL_PAIR(ds_ptr, void *, void *)

#endif /* DS__PAIR_H */
