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
#include <string.h>

#include <ds/hashset.h>

#define MAX(a, b)       ((a) > (b) ? (a) : (b))

#define DEFAULT_SLOTS   10
#define LOAD_FACTOR     4/5
#define SLOT_GROW       3/2

#define ITER_START      (-1)
#define ITER_FINISH     (-2)

struct ds_hashset_entry_s {
    struct ds_hashset_entry_s *next;
    ds_hash_t hash;
    char data[];
};

static int __hashset_init_slots(ds_hashset_t *hs, size_t initial_size) {
    hs->entries = 0;

    if (initial_size > 0) {
        hs->slots = ds_next_prime(initial_size);

        if (!(hs->data = (struct ds_hashset_entry_s **) hs->alloc_func(NULL,
                hs->slots * sizeof (struct ds_hashset_entry_s *)))) {
            return 0;
        }

        memset(hs->data, 0, hs->slots * sizeof (struct ds_hashset_entry_s *));
    } else {
        hs->slots = 0;
        hs->data = NULL;
    }

    return 1;
}

static void __hashset_free_entry(ds_hashset_t *hs, struct ds_hashset_entry_s *entry) {
    if (hs->clean_func != NULL)
        hs->clean_func(entry->data);
    hs->free_func(entry);
}

int
ds_hashset_create(ds_hashset_t *hs, size_t initial_size, ds_hash_func_t hash_func, ds_cmp_func_t cmp_func) {
    return ds_hashset_create_ext(hs, initial_size, hash_func, cmp_func, ds_alloc_def, ds_realloc_def, ds_free_def,
            NULL);
}

int
ds_hashset_create_ext(ds_hashset_t *hs, size_t initial_size, ds_hash_func_t hash_func, ds_cmp_func_t cmp_func,
        ds_alloc_func_t alloc_func, ds_realloc_func_t realloc_func, ds_free_func_t free_func,
        ds_clean_func_t clean_func) {
    hs->hash_func = hash_func;
    hs->cmp_func = cmp_func;
    hs->alloc_func = alloc_func;
    hs->realloc_func = realloc_func;
    hs->free_func = free_func;
    hs->clean_func = clean_func;

    return __hashset_init_slots(hs, initial_size);
}

void
ds_hashset_destroy(ds_hashset_t *hs) {
    for (int i = 0; i < hs->slots; i++) {
        struct ds_hashset_entry_s *entry = hs->data[i], *next;

        while (entry != NULL) {
            next = entry->next;
            __hashset_free_entry(hs, entry);
            entry = next;
        }
    }

    hs->entries = 0;
    hs->slots = 0;
    if (hs->data != NULL) {
        hs->free_func(hs->data);
        hs->data = NULL;
    }

    hs->hash_func = NULL;
    hs->cmp_func = NULL;
    hs->alloc_func = NULL;
    hs->realloc_func = NULL;
    hs->free_func = NULL;
}

int
ds_hashset_reserve(ds_hashset_t *hs, int req_slots) {
    if (hs->slots == 0) {
        if (!__hashset_init_slots(hs, MAX(DEFAULT_SLOTS, req_slots * (2 - LOAD_FACTOR) - 1))) {
            return 0;
        }
    } else if (req_slots >= hs->slots * LOAD_FACTOR) {
        int inc_slots = ds_next_prime(hs->slots * SLOT_GROW);
        int new_slot;
        void *slots;
        struct ds_hashset_entry_s **prev_next, *curr_entry, *tmp;

        assert(inc_slots >= hs->slots);

        if (!(slots = hs->realloc_func(hs->data, inc_slots * sizeof (struct ds_hashset_entry_s *))))
            return 0;

        hs->data = (struct ds_hashset_entry_s **) slots;

        memset(hs->data + hs->slots, 0, (inc_slots - hs->slots) * sizeof (struct ds_hashset_entry_s *));

        for (int slot = 0; slot < hs->slots; slot++) {
            prev_next = &hs->data[slot];
            curr_entry = hs->data[slot];

            while (curr_entry != NULL) {
                if (slot != (new_slot = curr_entry->hash % inc_slots)) {
                    tmp = curr_entry->next;

                    curr_entry->next = hs->data[new_slot];
                    hs->data[new_slot] = curr_entry;

                    *prev_next = tmp;
                    curr_entry = tmp;
                } else {
                    prev_next = &curr_entry->next;
                    curr_entry = curr_entry->next;
                }
            }
        }

        hs->slots = inc_slots;
    }

    return 1;
}

void*
ds_hashset_insert(ds_hashset_t *hs, const void *data, size_t size) {
    return ds_hashset_insert_ext(hs, data, size, 0);
}

void*
ds_hashset_insert_ext(ds_hashset_t *hs, const void *data, size_t size, size_t offset) {
    if (hs->slots == 0 && !__hashset_init_slots(hs, DEFAULT_SLOTS))
        return NULL;

    ds_hash_t hash = hs->hash_func(data);
    int slot = hash % hs->slots;
    struct ds_hashset_entry_s *entry = hs->data[slot];

    while (entry != NULL) {
        if (entry->hash == hash && hs->cmp_func(entry->data, data) == 0)
            break;
        entry = entry->next;
    }

    if (entry == NULL) {
        if (!ds_hashset_reserve(hs, hs->entries + 1))
            return NULL;

        slot = hash % hs->slots;

        if (!(entry = (struct ds_hashset_entry_s *) hs->alloc_func(NULL,
                offsetof(struct ds_hashset_entry_s, data) + size)))
            return NULL;

        memcpy((char *) entry->data + offset, (const char *) data + offset, size - offset);
        entry->hash = hash;
        entry->next = hs->data[slot];
        hs->data[slot] = entry;
        hs->entries++;
    }

    return entry->data;
}

void*
ds_hashset_search(const ds_hashset_t *hs, const void *data) {
    if (hs->slots == 0)
        return NULL;

    ds_hash_t hash = hs->hash_func(data);
    int slot = hash % hs->slots;
    struct ds_hashset_entry_s *entry = hs->data[slot];

    while (entry != NULL) {
        if (entry->hash == hash && hs->cmp_func(entry->data, data) == 0)
            break;
        entry = entry->next;
    }

    return entry ? entry->data : NULL;
}

void
ds_hashset_remove(ds_hashset_t *hs, const void *data) {
    if (hs->slots == 0)
        return;

    ds_hash_t hash = hs->hash_func(data);
    int slot = hash % hs->slots;
    struct ds_hashset_entry_s *entry = hs->data[slot];

    if (entry != NULL) {
        if (entry->hash == hash && hs->cmp_func(entry->data, data) == 0) {
            hs->data[slot] = entry->next;
            __hashset_free_entry(hs, entry);
            hs->entries--;
        } else {
            struct ds_hashset_entry_s *prev_entry;

            prev_entry = entry;
            entry = entry->next;

            while (entry != NULL) {
                if (entry->hash == hash && hs->cmp_func(entry->data, data) == 0) {
                    prev_entry->next = entry->next;
                    __hashset_free_entry(hs, entry);
                    hs->entries--;
                    break;
                }

                prev_entry = entry;
                entry = entry->next;
            }
        }
    }
}

void
ds_hashset_remove_direct(ds_hashset_t *hs, const void *data) {
    if (hs->slots == 0)
        return;

    struct ds_hashset_entry_s *data_entry = (struct ds_hashset_entry_s *) ((const char *) data -
            offsetof(struct ds_hashset_entry_s, data));
    int slot = data_entry->hash % hs->slots;
    struct ds_hashset_entry_s *iter_entry = hs->data[slot];

    if (iter_entry != NULL) {
        if (iter_entry == data_entry) {
            hs->data[slot] = data_entry->next;
            __hashset_free_entry(hs, data_entry);
            hs->entries--;
        } else {
            while (iter_entry->next != NULL) {
                if (iter_entry->next == data_entry) {
                    iter_entry->next = data_entry->next;
                    __hashset_free_entry(hs, data_entry);
                    hs->entries--;
                    break;
                }

                iter_entry = iter_entry->next;
            }
        }
    }

}

void
ds_hashset_clear(ds_hashset_t *hs) {
    for (int slot = 0; slot < hs->slots; slot++) {
        while (hs->data[slot] != NULL) {
            struct ds_hashset_entry_s *entry = hs->data[slot];

            hs->data[slot] = entry->next;
            __hashset_free_entry(hs, entry);
        }
    }

    hs->data = 0;
}

void
ds_hashset_iter_reset(ds_hashset_t *hs, ds_hashset_iter_t *iter) {
    iter->hashset = hs;
    iter->slot = ITER_START;
}

void*
ds_hashset_iter_next(ds_hashset_iter_t *iter) {
    if (iter->slot == ITER_FINISH)
        return NULL;

    if (iter->slot != ITER_START && iter->entry != NULL && iter->entry->next != NULL) {
        iter->entry = iter->entry->next;

        return iter->entry->data;
    }

    while (1) {
        iter->slot++;

        if (iter->slot == iter->hashset->slots) {
            iter->slot = ITER_FINISH;

            return NULL;
        }

        if (iter->hashset->data[iter->slot] != NULL) {
            iter->entry = iter->hashset->data[iter->slot];

            return iter->entry->data;
        }
    }
}
