/*!The Treasure Box Library
 * 
 * TBox is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or
 * (at your option) any later version.
 * 
 * TBox is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with TBox; 
 * If not, see <a href="http://www.gnu.org/licenses/"> http://www.gnu.org/licenses/</a>
 * 
 * Copyright (C) 2009 - 2017, ruki All rights reserved.
 *
 * @author      ruki
 * @file        strstr.c
 * @ingroup     libc
 *
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "string.h"
#ifdef TB_CONFIG_LIBC_HAVE_STRSTR
#   include <string.h>
#endif

/* //////////////////////////////////////////////////////////////////////////////////////
 * interfaces 
 */

#ifdef TB_CONFIG_LIBC_HAVE_STRSTR
tb_char_t* tb_strstr(tb_char_t const* s1, tb_char_t const* s2)
{
    tb_assert_and_check_return_val(s1 && s2, tb_null);
    return (tb_char_t*)strstr(s1, s2);
}
#else
tb_char_t* tb_strstr(tb_char_t const* s1, tb_char_t const* s2)
{
    // check
    tb_assert_and_check_return_val(s1 && s2, tb_null);

    // init
    __tb_register__ tb_char_t const* s = s1;
    __tb_register__ tb_char_t const* p = s2;

    // done
    do 
    {
        if (!*p) return (tb_char_t *)s1;
        if (*p == *s) 
        {
            ++p;
            ++s;
        } 
        else 
        {
            p = s2;
            if (!*s) return tb_null;
            s = ++s1;
        }

    } while (1);

    // no found
    return tb_null;
}
#endif
