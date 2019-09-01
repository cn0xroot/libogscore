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

lib_LTLIBRARIES = libogscrypt-1.0.la

if OGS_WINDOWS
no_undefined = -no-undefined
endif

libogscrypt_1_0_la_SOURCES = \
	ogs-aes.c \
	ogs-aes-cmac.c \
	ogs-sha1.c \
	ogs-sha1-hmac.c \
	ogs-sha2.c \
	ogs-sha2-hmac.c \
	$(NULL)

cryptincludedir=$(includedir)/ogslib-@OGS_API_VERSION@/crypt
cryptinclude_HEADERS = \
	ogs-aes.h \
	ogs-aes-cmac.h \
	ogs-sha1.h \
	ogs-sha1-hmac.h \
	ogs-sha2.h \
	ogs-sha2-hmac.h \
	$(NULL)

libogscrypt_1_0_la_LIBADD = \
	../core/libogscore-@OGS_API_VERSION@.la \
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
