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

#ifndef OGS_PROCESS_H
#define OGS_PROCESS_H

#if !defined(OGS_CORE_INSIDE)
#error "Only <ogs-core.h> can be included directly."
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if !defined(_WIN32)
typedef sem_t ogs_proc_mutex_t;
#define ogs_proc_mutex_wait sem_wait
#define ogs_proc_mutex_post sem_post
#define ogs_proc_mutex_delete sem_close
#else
typedef HANDLE ogs_proc_mutex_t;
#endif

ogs_proc_mutex_t *ogs_proc_mutex_create(int value);
int ogs_proc_mutex_timedwait(ogs_proc_mutex_t *mutex, ogs_time_t timeout);

#ifdef __cplusplus
}
#endif

#endif /* OGS_PROCESS_H */
