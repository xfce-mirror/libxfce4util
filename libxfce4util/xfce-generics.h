/* $Id$ */
/*-
 * Copyright (c) 2003-2004 Benedikt Meurer <benny@xfce.org>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef __LIBXFCE4UTIL_XFCE_GENERICS_H__
#define __LIBXFCE4UTIL_XFCE_GENERICS_H__

#include <glib.h>

G_BEGIN_DECLS;

#define XFCE_GENERIC_STACK(Type)                                            \
  struct                                                                    \
  {                                                                         \
    Type  *elements;                                                        \
    gint   nelements;                                                       \
    gint   top;                                                             \
  }


#define xfce_stack_new(StackType)                                           \
  ({                                                                        \
    StackType *stack;                                                       \
                                                                            \
    stack            = g_new (StackType, 1);                                \
    stack->elements  = g_malloc (20 * sizeof (*(stack->elements)));         \
    stack->nelements = 20;                                                  \
    stack->top       = -1;                                                  \
                                                                            \
    stack;                                                                  \
  })


#define xfce_stack_free(stack)                                              \
  G_STMT_START                                                              \
    {                                                                       \
      g_free (stack->elements);                                             \
      g_free (stack);                                                       \
    }                                                                       \
  G_STMT_END


#define xfce_stack_top(stack)                                               \
  ({                                                                        \
    g_assert (stack->top >= 0);                                             \
    stack->elements[stack->top];                                            \
  })


#define xfce_stack_pop(stack)                                               \
  G_STMT_START                                                              \
    {                                                                       \
      g_assert (stack->top > 0);                                            \
      stack->top--;                                                         \
    }                                                                       \
  G_STMT_END


#define xfce_stack_push(stack, value)                                       \
  G_STMT_START                                                              \
    {                                                                       \
      stack->top++;                                                         \
                                                                            \
      if (G_UNLIKELY (stack->top >= stack->nelements))                      \
        {                                                                   \
          stack->nelements *= 2;                                            \
          stack->elements = g_realloc (stack->elements,                     \
                                       stack->nelements * sizeof(value));   \
        }                                                                   \
                                                                            \
      stack->elements[stack->top] = value;                                  \
    }                                                                       \
  G_STMT_END

G_END_DECLS;

#endif /* !__LIBXFCE4UTIL_XFCE_GENERICS_H__ */
