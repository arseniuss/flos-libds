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

#ifndef TAP_H
#define TAP_H

#include <stdio.h>
#include <stdlib.h>

static int _tap_test = 0;

#define plan(N)       printf("1..%d\n", (N))
#define skip_all(S)   printf("1..0 # %s\n", (S))

#define pass(S)       (printf("ok %d - %s\n", ++_tap_test, (S)), 1)
#define fail(S)       (printf("not ok %d - %s\n", ++_tap_test, (S)), 0)

#define ok(P, S)      ((P) ? pass(S) : fail(S))
#define is(A, B, S)   ok((A) == (B), (S))
#define isnt(A, B, S) ok((A) != (B), (S))

#define skip(N, S)                                                                                                     \
    do {                                                                                                               \
        int _tap_skip = _tap_test + (N);                                                                               \
        while (_tap_test < _tap_skip)                                                                                  \
            printf("ok %d # SKIP %s\n", ++_tap_test, (S));                                                             \
    } while (0)

#ifndef NDEBUG
    #define diag(S)         fprintf(stderr, "# %s\n", (S))
    #define diagf(fmt, ...) fprintf(stderr, "# " fmt "\n", ##__VA_ARGS__)
    #define note(S)         fprintf(stdout, "# %s\n", (S))
#else
    #define diag(S)
    #define diagf(fmt, ...)
    #define note(S)
#endif

#define bail_out(S) (printf("Bail out! %s\n", (S)), exit(0))

#endif
