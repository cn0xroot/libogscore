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
#include "core/abts.h"

#define MAX_ITER 40000
#define MAX_RETRY 3
static int i = 0, x = 0;

static void thread_func(void *data)
{
    ogs_proc_mutex_t *mutex = data;
    int exitLoop = 1;

    /* slight delay to allow things to settle */
    ogs_usleep(ogs_time_from_sec(1));

    while (1)
    {
        ogs_proc_mutex_wait(mutex);
        if (i == MAX_ITER)
            exitLoop = 0;
        else
        {
            i++;
            x++;
        }
        ogs_proc_mutex_post(mutex);

        if (!exitLoop)
            break;
    }
}

static void test_proc_mutex(abts_case *tc, void *data)
{
    ogs_thread_t *t1, *t2, *t3, *t4;
    ogs_proc_mutex_t *mutex = NULL;

    mutex = ogs_proc_mutex_create(1);
    ABTS_PTR_NOTNULL(tc, mutex);

    i = 0;
    x = 0;

    t1 = ogs_thread_create(thread_func, mutex);
    ABTS_PTR_NOTNULL(tc, t1);
    t2 = ogs_thread_create(thread_func, mutex);
    ABTS_PTR_NOTNULL(tc, t2);
    t3 = ogs_thread_create(thread_func, mutex);
    ABTS_PTR_NOTNULL(tc, t3);
    t4 = ogs_thread_create(thread_func, mutex);
    ABTS_PTR_NOTNULL(tc, t4);

    ogs_thread_destroy(t1);
    ogs_thread_destroy(t2);
    ogs_thread_destroy(t3);
    ogs_thread_destroy(t4);

    ABTS_INT_EQUAL(tc, MAX_ITER, x);

    ogs_proc_mutex_delete(mutex);
}

static void test_proc_mutex_timed(abts_case *tc, void *data)
{
    static ogs_proc_mutex_t *timed_mutex;
    ogs_time_t timeout;
    ogs_time_t begin, end;
    int i;
    int rv;

    timed_mutex = ogs_proc_mutex_create(0);
    ABTS_PTR_NOTNULL(tc, timed_mutex);

    for (i = 0; i < MAX_RETRY; i++)
    {
        begin = ogs_get_monotonic_time();
        timeout = ogs_time_from_sec(1);
        rv = ogs_proc_mutex_timedwait(timed_mutex, timeout);
        end = ogs_get_monotonic_time();

        ABTS_INT_EQUAL(tc, OGS_TIMEUP, rv);
        ABTS_ASSERT(tc,
            "Timer returned too late", end - begin - timeout < 100000);
        break;
    }
    ABTS_ASSERT(tc, "Too many retries", i < MAX_RETRY);
    ogs_proc_mutex_delete(timed_mutex);
}

abts_suite *test_process(abts_suite *suite)
{
    suite = ADD_SUITE(suite)

    abts_run_test(suite, test_proc_mutex, NULL);
#if HAVE_SEM_TIMEDWAIT
    abts_run_test(suite, test_proc_mutex_timed, NULL);
#endif

    return suite;
}
