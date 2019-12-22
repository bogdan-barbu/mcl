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

#include <mcl/mcl.h>

#if !__STDC_NO_THREADS__
    #include <threads.h>
#else
    #define thread_local
#endif

class exception_desc = { sizeof (struct mcl_exception), NULL, NULL, NULL },
      bad_alloc_desc = { sizeof (struct mcl_exception), &exception_desc, NULL, NULL };

struct mcl_exception exception = { &(class){ sizeof exception, &exception_desc, NULL, NULL } },
                     bad_alloc = { &(class){ sizeof bad_alloc, &bad_alloc_desc, NULL, NULL } };

thread_local struct mcl_exceptionframe *mcl_exceptionstack;
