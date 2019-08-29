#                                                       -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

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

AC_PREREQ(2.69)

AC_INIT([ogslib],
	m4_esyscmd([build/git-version-gen .tarball-version]),
	[acetcom@gmail.com])

OGS_API_VERSION=1.0
AC_SUBST(OGS_API_VERSION)

AC_CONFIG_AUX_DIR([build])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_TESTDIR(tests)
AC_CANONICAL_HOST

AM_INIT_AUTOMAKE([1.13 foreign subdir-objects])
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])
AC_CONFIG_HEADER([src/config.h])

dnl Detect Host OS
case "$host" in
  *-mingw*)
    have_win=yes
  ;;
  *-*-darwin*)
    have_macosx=yes
  ;;
  *)
  ;;
esac

AM_CONDITIONAL(OGS_WINDOWS, test "$have_win" = yes)
AM_CONDITIONAL(OGS_MACOSX, test "$have_macosx" = yes)

AX_RECURSIVE_EVAL(["${libdir}"], [LIB_DIR])
AC_SUBST(LIB_DIR)

AH_TOP([
#ifndef OGSLIB_CONFIG_H
#define OGSLIB_CONFIG_H
])

AH_BOTTOM([
#endif /* OGSLIB_CONFIG_H */
])

dnl Checks for programs.
PKG_PROG_PKG_CONFIG
AC_PROG_CC
LT_INIT([win32-dll])

dnl Checks for headers.
AC_HEADER_STDC
AC_CHECK_HEADERS( \
    arpa/inet.h \
    ctype.h \
    errno.h \
    execinfo.h \
    fcntl.h \
    ifaddrs.h \
    netdb.h \
    pthread.h \
    semaphore.h \
    signal.h \
    stdarg.h \
    stddef.h \
    stdio.h \
    stdint.h \
    stdlib.h \
    string.h \
    strings.h \
    time.h \
    sys/time.h \
    unistd.h \
    net/if.h \
    netinet/in.h \
    netinet/in_systm.h \
    netinet/udp.h \
    netinet/tcp.h \
    sys/ioctl.h \
    sys/param.h \
    sys/random.h \
    sys/socket.h \
    sys/stat.h \
    limits.h \
    sys/syslimits.h \
    sys/types.h \
    sys/wait.h \
    sys/uio.h \
    stdbool.h \
)

##########################################
#### Checks for typedefs, structures, ####
####  and compiler characteristics.   ####
##########################################

dnl Checks for endianess.
AC_C_BIGENDIAN

dnl Checks for libraries.
AC_SEARCH_LIBS(gethostbyname, nsl)
AC_SEARCH_LIBS(gethostname, nsl)
AC_SEARCH_LIBS(socket, socket)
AC_SEARCH_LIBS(pthread_barrier_wait, pthread)
AC_SEARCH_LIBS(backtrace, execinfo)

dnl Checks for functions.
AC_CHECK_FUNCS(\
    arc4random arc4random_buf getrandom \
    clock_gettime localtime_r \
    getifaddrs \
    getenv putenv setenv unsetenv \
    eventfd \
    sem_timedwait \
)

AC_DECL_SYS_SIGLIST
AC_CHECK_FUNCS(\
    sigaction sigwait sigsuspend \
)

AC_FUNC_STRERROR_R()

AC_CHECK_FILE("/dev/urandom", [ DEV_RANDOM="/dev/urandom"])
AC_SUBST(DEV_RANDOM)
AC_DEFINE_UNQUOTED(OGS_DEV_RANDOM, "$DEV_RANDOM",
    [a suitable file to read random data from])

AC_CHECK_FUNCS(kqueue, [have_kqueue=yes], [have_kqueue=no])
AM_CONDITIONAL([WITH_KQUEUE], [test x$have_kqueue = xyes])
AC_CHECK_FUNCS(epoll_ctl, [have_epoll=yes], [have_epoll=no])
if test "x$have_epoll" = "xyes" ; then
    AC_DEFINE(HAVE_EPOLL, 1,
        [Define if your system supports the epoll system calls])
fi
AM_CONDITIONAL([WITH_EPOLL], [test x$have_epoll = xyes])

AC_CONFIG_FILES([ogscore-1.0.pc])
AC_CONFIG_FILES([ogscrypt-1.0.pc])
AC_CONFIG_FILES([src/core/Makefile])
AC_CONFIG_FILES([src/crypt/Makefile])
AC_CONFIG_FILES([src/Makefile])
AC_CONFIG_FILES([tests/core/Makefile])
AC_CONFIG_FILES([tests/crypt/Makefile])
AC_CONFIG_FILES([tests/Makefile])
AC_CONFIG_FILES([Makefile])
AC_OUTPUT

AC_MSG_NOTICE([OGSLIB of build options:

  version:              ${PACKAGE_VERSION}
  compiler:             ${CC}
  compiler flags:       ${CFLAGS}
  linker flags:         ${LDFLAGS}
  libraries:            ${LIBS}
  path prefix:          ${prefix}
  header prefix:        ${prefix}/include/ogslib
  library directory:    ${LIB_DIR}
])

