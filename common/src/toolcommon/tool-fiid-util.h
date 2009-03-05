/*
  Copyright (C) 2003-2009 FreeIPMI Core Team

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2, or (at your option)
  any later version.

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA
*/

#ifndef _TOOL_FIID_UTIL_H
#define _TOOL_FIID_UTIL_H

#include <freeipmi/freeipmi.h>

#define TOOL_FIID_OBJ_GET(__obj, __field, __val)                        \
  do {                                                                  \
    uint64_t __localval = 0, *__localval_ptr;                           \
    int8_t __ret;                                                       \
    __localval_ptr = (__val);                                           \
    if ((__ret = fiid_obj_get ((__obj), (__field), &__localval)) < 0)   \
      {                                                                 \
        pstdout_fprintf (state_data->pstate,                            \
                         stderr,                                        \
                         "fiid_obj_get: %s: %s\n",                      \
                         __field,                                       \
                         fiid_obj_errormsg ((__obj)));                  \
        goto cleanup;                                                   \
      }                                                                 \
    if (!__ret)                                                         \
      {                                                                 \
        pstdout_fprintf (state_data->pstate,                            \
                         stderr,                                        \
                         "fiid_obj_get: %s: no data set\n",             \
                         __field);                                      \
        goto cleanup;                                                   \
      }                                                                 \
    *__localval_ptr = __localval;                                       \
  } while (0)

#endif