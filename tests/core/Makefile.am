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

bin_PROGRAMS = testcore

testcore_SOURCES = \
	list-test.c pool-test.c strings-test.c time-test.c conv-test.c \
	log-test.c pkbuf-test.c memory-test.c rbtree-test.c timer-test.c \
	thread-test.c socket-test.c queue-test.c poll-test.c \
	tlv-test.c fsm-test.c hash-test.c \
	abts-main.c \
	$(NULL)

if !OGS_WINDOWS
testcore_SOURCES += process-test.c
endif

LDADD = \
    $(top_srcdir)/src/core/libogscore-@OGS_API_VERSION@.la \
	$(NULL)

DEPENDENCIES = \
    $(top_srcdir)/src/core/libogscore-@OGS_API_VERSION@.la \
	$(NULL)

AM_CPPFLAGS = \
    -I$(top_srcdir)/src \
	$(NULL)
