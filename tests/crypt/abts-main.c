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
#include "ogs-crypt.h"
#include "core/abts.h"

abts_suite *test_aes(abts_suite *suite);
abts_suite *test_sha(abts_suite *suite);

const struct testlist {
    abts_suite *(*func)(abts_suite *suite);
} alltests[] = {
    {test_aes},
    {test_sha},
    {NULL},
};

int main(int argc, const char **argv)
{
    int i;
    int debug = 0, trace = 0;
    const char *debug_mask = NULL, *trace_mask = NULL;
    abts_suite *suite = NULL;

    abts_init(argc, argv);

    for (i = 1; i < argc; i++) {
        /* abts_init(argc, argv) handles the following options */
        if (!strcmp(argv[i], "-v")) continue;
        if (!strcmp(argv[i], "-x")) continue;
        if (!strcmp(argv[i], "-l")) continue;
        if (!strcmp(argv[i], "-q")) continue;

        if (!strcmp(argv[i], "-d")) {
            debug = 1; debug_mask = argv[++i];
            continue;
        }
        if (!strcmp(argv[i], "-t")) {
            trace = 1; trace_mask = argv[++i];
            continue;
        }

        if (argv[i][0] == '-') {
            fprintf(stderr, "Invalid option: `%s'\n", argv[i]);
            exit(1);
        }
    }

    ogs_core_initialize();
    atexit(ogs_core_finalize);

    if (debug)
        ogs_log_set_mask_level(debug_mask, OGS_LOG_DEBUG);
    if (trace)
        ogs_log_set_mask_level(trace_mask, OGS_LOG_TRACE);

    for (i = 0; alltests[i].func; i++)
    {
        suite = alltests[i].func(suite);
    }

    return abts_report(suite);
}
