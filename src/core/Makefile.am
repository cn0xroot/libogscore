# Copyright (C) 2019 by Sukchan Lee <acetcom@gmail.com>

# This file is part of Open5GS.

# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU Affero General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.

lib_LTLIBRARIES = libogscore-1.0.la 

if OGS_WINDOWS
SYS_LIBS = -lws2_32
no_undefined = -no-undefined
endif

libogscore_1_0_la_SOURCES = \
	ogs-abort.c ogs-errno.c \
	ogs-strings.c ogs-time.c ogs-conv.c \
	ogs-log.c ogs-pkbuf.c ogs-memory.c ogs-rbtree.c ogs-timer.c \
	ogs-rand.c ogs-thread.c ogs-signal.c ogs-process.c \
	ogs-sockaddr.c ogs-socket.c ogs-sockpair.c ogs-socknode.c \
	ogs-udp.c ogs-tcp.c \
	ogs-queue.c ogs-select.c ogs-poll.c ogs-notify.c \
	ogs-tlv.c ogs-env.c ogs-fsm.c ogs-hash.c ogs-misc.c ogs-getopt.c\
	ogs-core.c abts.h abts.c \
	$(NULL)

coreincludedir=$(includedir)/ogslib-@OGS_API_VERSION@/core
coreinclude_HEADERS = \
	ogs-compat.h ogs-macros.h \
	ogs-pool.h ogs-list.h \
	ogs-abort.h ogs-errno.h \
	ogs-strings.h ogs-time.h ogs-conv.h \
	ogs-log.h ogs-pkbuf.h ogs-memory.h ogs-rbtree.h ogs-timer.h \
	ogs-rand.h ogs-thread.h ogs-signal.h ogs-process.h \
	ogs-sockaddr.h ogs-socket.h ogs-sockpair.h ogs-socknode.h \
	ogs-udp.h ogs-tcp.h \
	ogs-queue.h ogs-poll.h ogs-notify.h \
	ogs-tlv.h ogs-env.h ogs-fsm.h ogs-hash.h ogs-misc.h ogs-getopt.h \
	abts.h \
	$(NULL)

NOINST_H_FILES = \
	ogs-poll-private.h \
	$(NULL)

if WITH_KQUEUE
libogscore_1_0_la_SOURCES += ogs-kqueue.c
endif

if WITH_EPOLL
libogscore_1_0_la_SOURCES += ogs-epoll.c
endif

libogscore_1_0_la_LIBADD = \
	$(SYS_LIBS) \
	$(NULL)

AM_LDFLAGS = \
	$(no_undefined) \
	$(NULL)

AM_CFLAGS = \
	-DOGS_CORE_COMPILATION \
	-Wall -Werror \
	$(NULL)

AM_CPPFLAGS = \
    -I$(top_srcdir)/src \
	$(NULL)

EXTRA_DIST = \
	$(NOINST_H_FILES) \
	$(NULL)
