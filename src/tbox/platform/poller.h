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
 * @file        poller.h
 * @ingroup     platform
 *
 */
#ifndef TB_PLATFORM_POLLER_H
#define TB_PLATFORM_POLLER_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "socket.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_enter__

/* //////////////////////////////////////////////////////////////////////////////////////
 * types
 */

/// the poller event enum, only for sock
typedef enum __tb_poller_event_e
{
    // the waited events
    TB_POLLER_EVENT_NONE        = TB_SOCKET_EVENT_NONE
,   TB_POLLER_EVENT_CONN        = TB_SOCKET_EVENT_CONN
,   TB_POLLER_EVENT_ACPT        = TB_SOCKET_EVENT_ACPT
,   TB_POLLER_EVENT_RECV        = TB_SOCKET_EVENT_RECV
,   TB_POLLER_EVENT_SEND        = TB_SOCKET_EVENT_SEND
,   TB_POLLER_EVENT_EALL        = TB_SOCKET_EVENT_EALL

    // the event flags after waiting
,   TB_POLLER_EVENT_CLEAR       = 0x0010 //!< edge trigger. after the event is retrieved by the user, its state is reset
,   TB_POLLER_EVENT_ONESHOT     = 0x0020 //!< causes the event to return only the first occurrence of the filter being triggered

    /*! the event flag will be marked if the connection be closed in the edge trigger (TB_POLLER_EVENT_CLEAR)
     *
     * be similar to epoll.EPOLLRDHUP and kqueue.EV_EOF
     */
,   TB_POLLER_EVENT_EOF         = 0x0100

}tb_poller_event_e;

/// the poller ref type
typedef __tb_typeref__(poller);

/*! the poller event func type
 *
 * @param poller    the poller
 * @param sock      the socket
 * @param events    the poller events
 * @param priv      the user private data for this socket
 */
typedef tb_void_t   (*tb_poller_event_func_t)(tb_poller_ref_t poller, tb_socket_ref_t sock, tb_size_t events, tb_cpointer_t priv);

/* //////////////////////////////////////////////////////////////////////////////////////
 * interfaces
 */

/*! init poller
 *
 * @param priv      the user private data 
 *
 * @param poller    the poller
 */
tb_poller_ref_t     tb_poller_init(tb_cpointer_t priv);

/*! exit poller
 *
 * @param poller    the poller
 */
tb_void_t           tb_poller_exit(tb_poller_ref_t poller);

/*! clear all sockets
 *
 * @param poller    the poller
 */
tb_void_t           tb_poller_clear(tb_poller_ref_t poller);

/*! get the user private data
 *
 * @param poller    the poller
 *
 * @return          the user private data
 */
tb_cpointer_t       tb_poller_priv(tb_poller_ref_t poller);

/*! kill all waited events, tb_poller_wait() will return -1
 *
 * @param poller    the poller
 */
tb_void_t           tb_poller_kill(tb_poller_ref_t poller);

/*! spak the poller, break the tb_poller_wait() and return all events
 *
 * @param poller    the poller
 */
tb_void_t           tb_poller_spak(tb_poller_ref_t poller);

/*! the events(clear, oneshot, ..) is supported for the poller?
 *
 * @param poller    the poller
 * @param events    the poller events
 *
 * @return          tb_true or tb_false
 */
tb_bool_t           tb_poller_support(tb_poller_ref_t poller, tb_size_t events);

/*! insert socket to poller
 *
 * @param poller    the poller
 * @param sock      the socket
 * @param events    the poller events
 * @param priv      the private data
 *
 * @return          tb_true or tb_false
 */
tb_bool_t           tb_poller_insert(tb_poller_ref_t poller, tb_socket_ref_t sock, tb_size_t events, tb_cpointer_t priv);

/*! remove socket from poller
 *
 * @param poller    the poller
 * @param aioo      the aioo
 *
 * @return          tb_true or tb_false
 */
tb_bool_t           tb_poller_remove(tb_poller_ref_t poller, tb_socket_ref_t sock);

/*! modify events for the given socket
 *
 * @param poller    the poller
 * @param sock      the socket
 * @param events    the poller events
 * @param priv      the private data
 *
 * @return          tb_true or tb_false
 */
tb_bool_t           tb_poller_modify(tb_poller_ref_t poller, tb_socket_ref_t sock, tb_size_t events, tb_cpointer_t priv);

/*! wait all sockets
 *
 * @param poller    the poller
 * @param func      the events function
 * @param timeout   the timeout, infinity: -1
 *
 * @return          > 0: the events number, 0: timeout, -1: failed
 */
tb_long_t           tb_poller_wait(tb_poller_ref_t poller, tb_poller_event_func_t func, tb_long_t timeout);

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_leave__

#endif
