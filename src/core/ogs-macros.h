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

#if !defined(OGS_CORE_INSIDE) && !defined(OGS_CORE_COMPILATION)
#error "This header cannot be included directly."
#endif

#ifndef OGS_MACROS_H
#define OGS_MACROS_H

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __GNUC__
#define OGS_GNUC_CHECK_VERSION(major, minor) \
    ((__GNUC__ > (major)) || \
     ((__GNUC__ == (major)) && (__GNUC_MINOR__ >= (minor))))
#else
#define OGS_GNUC_CHECK_VERSION(major, minor) 0
#endif

#if defined(_MSC_VER)
#define ogs_inline __inline
#else
#define ogs_inline __inline__
#endif

#if defined(_WIN32)
#define OGS_FUNC __FUNCTION__
#elif defined(__STDC_VERSION__) && __STDC_VERSION__ < 199901L
#define OGS_FUNC __FUNCTION__
#else
#define OGS_FUNC __func__
#endif

#if defined(__GNUC__)
#define ogs_likely(x) __builtin_expect (!!(x), 1)
#define ogs_unlikely(x) __builtin_expect (!!(x), 0)
#else
#define ogs_likely(v) v
#define ogs_unlikely(v) v
#endif

#if defined(__clang__)
#define OGS_GNUC_PRINTF(f, v) __attribute__ ((format (printf, f, v)))
#elif OGS_GNUC_CHECK_VERSION(4, 4)
#define OGS_GNUC_PRINTF(f, v) __attribute__ ((format (gnu_printf, f, v)))
#else
#define OGS_GNUC_PRINTF(f, v) 
#endif

#if defined(_WIN32)

#   if BYTE_ORDER == LITTLE_ENDIAN
#       define OGS_WORDS_BIGENDIAN 0
#   elif BYTE_ORDER == BIG_ENDIAN
#       define OGS_WORDS_BIGENDIAN 1
#   else
#       error byte order not supported
#   endif

#   define htole16(x) (x)
#   define htole32(x) (x)
#   define htole64(x) (x)
#   define le16toh(x) (x)
#   define le32toh(x) (x)
#   define le64toh(x) (x)

#   define htobe16(x) htons((x))
#   define htobe32(x) htonl((x))
#   define htobe64(x) htonll((x))
#   define be16toh(x) ntohs((x))
#   define be32toh(x) ntohl((x))
#   define be64toh(x) ntohll((x))

#   define __BYTE_ORDER    BYTE_ORDER
#   define __BIG_ENDIAN    BIG_ENDIAN
#   define __LITTLE_ENDIAN LITTLE_ENDIAN

#elif defined(__linux__) || defined(__CYGWIN__)
#   include <endian.h>

#elif defined(__APPLE__)
#   include <libkern/OSByteOrder.h>

#   define htole16(x) OSSwapHostToLittleInt16((x))
#   define htole32(x) OSSwapHostToLittleInt32((x))
#   define htole64(x) OSSwapHostToLittleInt64((x))
#   define le16toh(x) OSSwapLittleToHostInt16((x))
#   define le32toh(x) OSSwapLittleToHostInt32((x))
#   define le64toh(x) OSSwapLittleToHostInt64((x))

#   define htobe16(x) OSSwapHostToBigInt16((x))
#   define htobe32(x) OSSwapHostToBigInt32((x))
#   define htobe64(x) OSSwapHostToBigInt64((x))
#   define be16toh(x) OSSwapBigToHostInt16((x))
#   define be32toh(x) OSSwapBigToHostInt32((x))
#   define be64toh(x) OSSwapBigToHostInt64((x))

#   define __BYTE_ORDER    BYTE_ORDER
#   define __BIG_ENDIAN    BIG_ENDIAN
#   define __LITTLE_ENDIAN LITTLE_ENDIAN

#elif defined(__FreeBSD__)
#   include <sys/endian.h>

#   define le16toh(x) letoh16(x)
#   define le32toh(x) letoh32(x)
#   define le64toh(x) letoh64(x)

#   define be16toh(x) betoh16(x)
#   define be32toh(x) betoh32(x)
#   define be64toh(x) betoh64(x)

#else
#   error platform not supported
#endif

#if __BYTE_ORDER == __BIG_ENDIAN
#define ED2(x1, x2) x1 x2
#define ED3(x1, x2, x3) x1 x2 x3
#define ED4(x1, x2, x3, x4) x1 x2 x3 x4
#define ED5(x1, x2, x3, x4, x5) x1 x2 x3 x4 x5
#define ED6(x1, x2, x3, x4, x5, x6) x1 x2 x3 x4 x5 x6
#define ED7(x1, x2, x3, x4, x5, x6, x7) x1 x2 x3 x4 x5 x6 x7
#define ED8(x1, x2, x3, x4, x5, x6, x7, x8) x1 x2 x3 x4 x5 x6 x7 x8
#else
#define ED2(x1, x2) x2 x1
#define ED3(x1, x2, x3) x3 x2 x1
#define ED4(x1, x2, x3, x4) x4 x3 x2 x1
#define ED5(x1, x2, x3, x4, x5) x5 x4 x3 x2 x1
#define ED6(x1, x2, x3, x4, x5, x6) x6 x5 x4 x3 x2 x1
#define ED7(x1, x2, x3, x4, x5, x6, x7) x7 x6 x5 x4 x3 x2 x1
#define ED8(x1, x2, x3, x4, x5, x6, x7, x8) x8 x7 x6 x5 x4 x3 x2 x1
#endif

#define OGS_STATIC_ASSERT(expr) \
    typedef char dummy_for_ogs_static_assert##__LINE__[(expr) ? 1 : -1]

#define OGS_ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

#define OGS_STRINGIFY(n)            OGS_STRINGIFY_HELPER(n)
#define OGS_STRINGIFY_HELPER(n)     #n

#define OGS_PASTE(n1, n2)           OGS_PASTE_HELPER(n1, n2)
#define OGS_PASTE_HELPER(n1, n2)    n1##n2

#define INET_NTOP(src, dst) \
    inet_ntop(AF_INET, (void *)(uintptr_t)(src), (dst), INET_ADDRSTRLEN)
#define INET6_NTOP(src, dst) \
    inet_ntop(AF_INET6, (void *)(src), (dst), INET6_ADDRSTRLEN)

#define ogs_max(x , y)  (((x) > (y)) ? (x) : (y))
#define ogs_min(x , y)  (((x) < (y)) ? (x) : (y))

#ifdef __cplusplus
}
#endif

#endif /* OGS_MACROS_H */
