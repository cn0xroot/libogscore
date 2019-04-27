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

ogs_proc_mutex_t *ogs_proc_mutex_create(int value)
{
#if HAVE_SEMAPHORE_H
    ogs_proc_mutex_t *mutex = NULL;
    struct timeval tv;
#define SEM_NAME_LEN 64
    char semname[SEM_NAME_LEN];
    ogs_time_t now;

    ogs_gettimeofday(&tv);
    now = ogs_time_from_sec(tv.tv_sec) + tv.tv_usec;

    ogs_snprintf(semname, SEM_NAME_LEN, "/ogssem%lld", (long long)now);
    mutex = sem_open(semname, O_CREAT | O_EXCL, 0644, value);
    ogs_assert(mutex);

    sem_unlink(semname);

    return mutex;
#else
    ogs_assert_if_reached();
    return OGS_ERROR;
#endif
}

int ogs_proc_mutex_timedwait(ogs_proc_mutex_t *mutex, ogs_time_t timeout)
{
#if HAVE_SEM_TIMEDWAIT
    int rv;
    struct timespec to;
    struct timeval tv;
    ogs_time_t usec;
    ogs_assert(mutex);

    ogs_gettimeofday(&tv);

    usec = ogs_time_from_sec(tv.tv_sec) + tv.tv_usec + timeout;

    to.tv_sec = ogs_time_sec(usec);
    to.tv_nsec = ogs_time_usec(usec) * 1000;
    rv = sem_timedwait(mutex, &to);
    if (rv == -1 && ETIMEDOUT == errno)
    {
        return OGS_TIMEUP;
    }
    return rv;
#else
    ogs_assert_if_reached();
    return OGS_ERROR;
#endif
}
