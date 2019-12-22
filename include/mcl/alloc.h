/*
 * Copyright 2019 Bogdan Barbu
 *
 * This file is part of MCL.
 *
 * MCL is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * MCL is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with MCL.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef MCL_ALLOC_H
#define MCL_ALLOC_H

#include <assert.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include <mcl/class.h>
#include <mcl/exception.h>

extern class bad_alloc_desc;
extern struct mcl_exception bad_alloc;

static void mcl_construct(void *const, class *, va_list);

static inline void *new(class *const desc, ...) {
    void *obj;
    va_list args;

    assert(desc != NULL);

    if ((obj = malloc(sizeof desc->size)) == NULL)
        throw(&bad_alloc);
    memcpy(obj, &desc, sizeof desc);

    va_start(args, desc);
    try {
        mcl_construct(obj, desc, args);
    } finally {
        va_end(args);
    }

    return obj;
}

static inline void delete(void *const obj) {
    if (obj == NULL)
        return;

    for (class *desc = typeid(obj); desc != NULL; desc = desc->parent)
        if (desc->dtor != NULL)
            desc->dtor(obj);

    free(obj);
}

void mcl_construct(void *const obj, class *desc, va_list args) {
    if (desc == NULL)
        return;

    try {
        mcl_construct(obj, desc->parent, args);
        if (desc->ctor != NULL)
            desc->ctor(obj, args);
    } catchall {
        if (desc->dtor != NULL)
            desc->dtor(obj);
        rethrow;
    }
}

#endif
