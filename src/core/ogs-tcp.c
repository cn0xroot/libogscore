/*
 * Copyright (C) 2019 by Sukchan Lee <acetcom@gmail.com>
 *
 * This file is part of Open5GS.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "ogs-core.h"

#undef OGS_LOG_DOMAIN
#define OGS_LOG_DOMAIN __ogs_sock_domain

ogs_sock_t *ogs_tcp_server(ogs_sockaddr_t *sa_list)
{
    int rv;
    ogs_sock_t *new = NULL;
    ogs_sockaddr_t *addr;
    char buf[OGS_ADDRSTRLEN];

    ogs_assert(sa_list);

    addr = sa_list;
    while(addr) {
        new = ogs_sock_socket(addr->c_sa_family, SOCK_STREAM, IPPROTO_TCP);
        if (new) {
            rv = ogs_listen_reusable(new->fd);
            ogs_assert(rv == OGS_OK);

            if (ogs_sock_bind(new, addr) == OGS_OK) {
                ogs_debug("tcp_server() [%s]:%d",
                        OGS_ADDR(addr, buf), OGS_PORT(addr));
                break;
            }

            ogs_sock_destroy(new);
        }

        addr = addr->next;
    }

    if (addr == NULL) {
        ogs_log_message(OGS_LOG_ERROR, ogs_socket_errno,
                "tcp_server() [%s]:%d failed",
                OGS_ADDR(sa_list, buf), OGS_PORT(sa_list));
        return NULL;
    }

    rv = ogs_sock_listen(new);
    ogs_assert(rv == OGS_OK);

    return new;
}

ogs_sock_t *ogs_tcp_client(ogs_sockaddr_t *sa_list)
{
    ogs_sock_t *new = NULL;
    ogs_sockaddr_t *addr;
    char buf[OGS_ADDRSTRLEN];

    ogs_assert(sa_list);

    addr = sa_list;
    while (addr) {
        new = ogs_sock_socket(addr->c_sa_family, SOCK_STREAM, IPPROTO_TCP);
        if (new) {
            if (ogs_sock_connect(new, addr) == OGS_OK) {
                ogs_debug("tcp_client() [%s]:%d",
                        OGS_ADDR(addr, buf), OGS_PORT(addr));
                break;
            }

            ogs_sock_destroy(new);
        }

        addr = addr->next;
    }

    if (addr == NULL) {
        ogs_log_message(OGS_LOG_ERROR, ogs_socket_errno,
                "tcp_client() [%s]:%d failed",
                OGS_ADDR(addr, buf), OGS_PORT(addr));
        return NULL;
    }

    return new;
}
