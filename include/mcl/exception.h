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

#ifndef MCL_EXCEPTION_H
#define MCL_EXCEPTION_H

#if !__STDC__ || defined(__cplusplus) || !__STDC_HOSTED__ || __STDC_VERSION__ < 199901L
    #error Exception handling requires a hosted C99+ implementation.
#endif

#include <assert.h>
#include <setjmp.h>
#include <stdbool.h>
#include <stdlib.h>
#if __STDC_VERSION__ >= 201112L
    #include <stdnoreturn.h>
    #if !__STDC_NO_THREADS__
        #include <threads.h>
    #else
        #define thread_local
    #endif
#else
    #define noreturn
    #define thread_local
#endif
#include <string.h>

#include <mcl/class.h>

#define try \
    for (bool mcl_handled = (mcl_exceptionframe_push(&mcl_exceptionstack), false); !mcl_handled;                         \
            mcl_advance(mcl_exceptionstack) ? mcl_handled = true, mcl_exceptionframe_pop(&mcl_exceptionstack) : (void)0) \
        if (setjmp(mcl_exceptionstack->context) == MCL_TRY)

#define catch(e) \
        else if (mcl_exceptionstack->state == MCL_CATCH && mcl_ishandler(mcl_exceptionstack, (e)) && \
                !(mcl_exceptionstack->thrown = false))

#define catchall \
        else if (mcl_exceptionstack->state == MCL_CATCH && !(mcl_exceptionstack->thrown = false))

#define finally \
        else if (mcl_exceptionstack->state == MCL_FINALLY)

#define throw(e) \
    do { \
        assert(mcl_exceptionstack != NULL);  \
        mcl_throw(&mcl_exceptionstack, (e)); \
    } while (false)

#define rethrow throw(mcl_exceptionstack->exception)

enum mcl_exceptionstate {
    MCL_TRY,
    MCL_CATCH,
    MCL_FINALLY
};

struct mcl_exception {
    class *desc;
};

struct mcl_exceptionframe {
    struct mcl_exceptionframe *prev;
    enum mcl_exceptionstate state;
    jmp_buf context;
    void *exception;
    bool thrown;
};

extern class exception_desc;
extern struct mcl_exception exception;

extern thread_local struct mcl_exceptionframe *mcl_exceptionstack;

static void mcl_exceptionframe_push(struct mcl_exceptionframe **const stack) {
    struct mcl_exceptionframe *const top = malloc(sizeof *top);

    if (top == NULL)
        exit(EXIT_FAILURE);

    top->prev = *stack;
    top->state = MCL_TRY;
    top->exception = NULL;
    top->thrown = false;
    *stack = top;
}

static void mcl_exceptionframe_pop(struct mcl_exceptionframe **const stack) {
    struct mcl_exceptionframe *const top = *stack;

    *stack = top->prev;
    free(top);
}

static noreturn void mcl_throw(struct mcl_exceptionframe **const stack, void *const e) {
    assert(e != NULL);

    if ((*stack)->exception != e) {
        if (((*stack)->exception = malloc(typeid(e)->size)) == NULL)
            exit(EXIT_FAILURE);
        memcpy((*stack)->exception, e, typeid(e)->size);
    }

    (*stack)->thrown = true;

    switch ((*stack)->state) {
    case MCL_TRY:
        longjmp((*stack)->context, (*stack)->state = MCL_CATCH);

    case MCL_CATCH:
        longjmp((*stack)->context, (*stack)->state = MCL_FINALLY);

    case MCL_FINALLY:
        assert((*stack)->prev != NULL);

        (*stack)->prev->exception = (*stack)->exception;
        (*stack)->prev->thrown = (*stack)->thrown;
        mcl_exceptionframe_pop(stack);
        longjmp((*stack)->context, (*stack)->state = MCL_CATCH);
    }

    exit(EXIT_FAILURE);
}

static bool mcl_ishandler(struct mcl_exceptionframe *const stack, void *const e) {
    for (class *desc = typeid(stack->exception); desc != NULL; desc = desc-> parent)
        if (desc == typeid(e)) {
            memcpy(e, stack->exception, typeid(e)->size);
            return true;
        }

    return false;
}

static bool mcl_advance(struct mcl_exceptionframe *const stack) {
    if (stack->state != MCL_FINALLY)
        longjmp(stack->context, mcl_exceptionstack->state = MCL_FINALLY);

    if (stack->thrown)
        rethrow;

    free(stack->exception);
    return true;
}

#if __STDC_VERSION__ < 201112L
    #undef noreturn
    #undef thread_local
#elif __STDC_NO_THREADS__
    #undef thread_local
#endif

#endif
