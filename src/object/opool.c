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
 * Copyright (C) 2009 - 2012, ruki All rights reserved.
 *
 * @author		ruki
 * @file		opool.c
 * @ingroup 	object
 *
 */

/* ///////////////////////////////////////////////////////////////////////
 * includes
 */
#include "object.h"

/* ///////////////////////////////////////////////////////////////////////
 * macros
 */
#ifdef __tb_small__
# 	define TB_OPOOL_GROW 		(256 * 1024)
#else
# 	define TB_OPOOL_GROW 		(512 * 1024)
#endif

/* ///////////////////////////////////////////////////////////////////////
 * globals
 */

// the pool
static tb_handle_t 				g_pool = tb_null;

// the mutx
static tb_handle_t 				g_mutx = tb_null;

/* ///////////////////////////////////////////////////////////////////////
 * implementation
 */
tb_bool_t tb_opool_init()
{
	// init mutx
	if (!g_mutx) g_mutx = tb_mutex_init(tb_null);
	tb_assert_and_check_return_val(g_mutx, tb_false);

	// init pool
	if (!g_pool) g_pool = tb_spool_init(TB_OPOOL_GROW, 0);
	tb_assert_and_check_return_val(g_pool, tb_false);

	// ok
	return tb_true;
}
tb_void_t tb_opool_exit()
{
	// dump
#ifdef __tb_debug__
	tb_opool_dump();
#endif

	// enter
	if (g_mutx) tb_mutex_enter(g_mutx);

	// exit pool
	if (g_pool) tb_spool_exit(g_pool);
	g_pool = tb_null;

	// leave
	if (g_mutx) tb_mutex_leave(g_mutx);

	// exit mutx
	if (g_mutx) tb_mutex_exit(g_mutx);
	g_mutx = tb_null;
}
tb_void_t tb_opool_clear()
{
	// enter
	if (g_mutx) tb_mutex_enter(g_mutx);

	// clear pool
	if (g_pool) tb_spool_clear(g_pool);

	// leave
	if (g_mutx) tb_mutex_leave(g_mutx);
}
#ifndef __tb_debug__
tb_object_t* tb_opool_get_impl(tb_size_t size, tb_size_t flag, tb_size_t type)
#else
tb_object_t* tb_opool_get_impl(tb_size_t size, tb_size_t flag, tb_size_t type, tb_char_t const* func, tb_size_t line, tb_char_t const* file)
#endif
{
	// check
	tb_assert_and_check_return_val(size && type, tb_null);

	// enter
	if (g_mutx) tb_mutex_enter(g_mutx);

	// make object
#ifndef __tb_debug__
	tb_object_t* object = g_pool? tb_spool_malloc0(g_pool, size) : tb_null;
#else
	tb_object_t* object = g_pool? tb_spool_malloc0_impl(g_pool, size, func, line, file) : tb_null;
#endif

	// init object
	if (object) 
	{
		if (!tb_object_init(object, flag, type)) 
		{
#ifndef __tb_debug__
			tb_spool_free(g_pool, object);
#else
			tb_spool_free_impl(g_pool, object, func, line, file);
#endif
			object = tb_null;
		}
	}

	// leave
	if (g_mutx) tb_mutex_leave(g_mutx);

	// ok?
	return object;
}
#ifndef __tb_debug__
tb_void_t tb_opool_del_impl(tb_object_t* object)
#else
tb_void_t tb_opool_del_impl(tb_object_t* object, tb_char_t const* func, tb_size_t line, tb_char_t const* file)
#endif
{
	// check
	tb_assert_and_check_return(object);

	// enter
	if (g_mutx) tb_mutex_enter(g_mutx);

	// exit object
#ifndef __tb_debug__
	if (g_pool) tb_spool_free(g_pool, object);
#else
	if (g_pool) tb_spool_free_impl(g_pool, object, func, line, file);
#endif

	// leave
	if (g_mutx) tb_mutex_leave(g_mutx);
}
#ifdef __tb_debug__
tb_void_t tb_opool_dump()
{
	// enter
	if (g_mutx) tb_mutex_enter(g_mutx);

	// dump
	if (g_pool) tb_spool_dump(g_pool);

	// leave
	if (g_mutx) tb_mutex_leave(g_mutx);
}
#endif