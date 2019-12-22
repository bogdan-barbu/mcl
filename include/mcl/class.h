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

#ifndef MCL_CLASS_H
#define MCL_CLASS_H

#include <assert.h>
#include <stdarg.h>
#include <stddef.h>

#define VIRTUAL_CALL(obj, func, ...) (class *)(obj)->vtable[func](__VA_ARGS__)

typedef const struct mcl_class {
    size_t size;
    const struct mcl_class *parent;
    void (*ctor)(void *const, va_list);
    void (*dtor)(void *const);
    void (*vtable[])();
} class;

static inline class *typeid(void *const obj) {
    assert(obj != NULL && *(class **)obj != NULL);
    return *(class **)obj;
}

#endif
