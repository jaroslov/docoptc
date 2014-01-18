/*

Copyright (c) 2014, Jacob N. Smith
All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

    1.  Redistributions of source code must retain the above copyright notice,
        this list of conditions and the following disclaimer.

    2.  Redistributions in binary form must reproduce the above copyright notice,
        this list of conditions and the following disclaimer in the documentation
        and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
OF THE POSSIBILITY OF SUCH DAMAGE.

*/

#define DOCOPT_C
//#define DOCOPT_DEBUG
#include "docopt.h"

struct docopt_check
{
    const char* key;
    const int   index;
    const char* value;
};

struct docopt_test
{
    const int                   usage;
    const char*                 args[30];
    const struct docopt_check   checks[30];
};

const char *usages[] =
{
    /* 0 */
    "Usage: test [options]\n"
    "Options:\n"
    "  -x\n",
    "Usage: test [options]\n"
    "Options:\n"
    "  --yy\n",
    "Usage: test [options]\n"
    "Options:\n"
    "  -x, --yy\n",
    "Usage: test [options]\n"
    "Options:\n"
    "  -x --yy\n",
    "Usage: test [options]\n"
    "Options:\n"
    "  --yy, -x\n",
    "Usage: test [options]\n"
    "Options:\n"
    "  --yy -x\n",
    /* 6 */
    "Usage: test [options]\n"
    "Options:\n"
    "  -xXXX\n",
    "Usage: test [options]\n"
    "Options:\n"
    "  --yy\n",
    "Usage: test [options]\n"
    "Options:\n"
    "  -xXXX, --yy\n",
    "Usage: test [options]\n"
    "Options:\n"
    "  -xXXX --yy\n",
    "Usage: test [options]\n"
    "Options:\n"
    "  --yy, -xXXX\n",
    "Usage: test [options]\n"
    "Options:\n"
    "  --yy -xXXX\n",
    /* 12 */
    "Usage: test [options]\n"
    "Options:\n"
    "  -x XXX\n",
    "Usage: test [options]\n"
    "Options:\n"
    "  --yy\n",
    "Usage: test [options]\n"
    "Options:\n"
    "  -x XXX, --yy\n",
    "Usage: test [options]\n"
    "Options:\n"
    "  -x XXX --yy\n",
    "Usage: test [options]\n"
    "Options:\n"
    "  --yy, -x XXX\n",
    "Usage: test [options]\n"
    "Options:\n"
    "  --yy -x XXX\n",
    /* 18 */
    "Usage: test [options]\n"
    "Options:\n"
    "  -x\n",
    "Usage: test [options]\n"
    "Options:\n"
    "  --yy=YYY\n",
    "Usage: test [options]\n"
    "Options:\n"
    "  -x, --yy=YYY\n",
    "Usage: test [options]\n"
    "Options:\n"
    "  -x --yy=YYY\n",
    "Usage: test [options]\n"
    "Options:\n"
    "  --yy=YYY, -x\n",
    "Usage: test [options]\n"
    "Options:\n"
    "  --yy=YYY -x\n",
    /* 24 */
    "Usage: test [options]\n"
    "Options:\n"
    "  -x\n",
    "Usage: test [options]\n"
    "Options:\n"
    "  --yy YYY\n",
    "Usage: test [options]\n"
    "Options:\n"
    "  -x, --yy YYY\n",
    "Usage: test [options]\n"
    "Options:\n"
    "  -x --yy YYY\n",
    "Usage: test [options]\n"
    "Options:\n"
    "  --yy YYY, -x\n",
    "Usage: test [options]\n"
    "Options:\n"
    "  --yy YYY -x\n",
    /* 30 */
    "Usage: test [options]\n"
    "Options:\n"
    "  -xXXX\n",
    "Usage: test [options]\n"
    "Options:\n"
    "  --yy=YYY\n",
    "Usage: test [options]\n"
    "Options:\n"
    "  -xXXX, --yy=YYY\n",
    "Usage: test [options]\n"
    "Options:\n"
    "  -xXXX --yy=YYY\n",
    "Usage: test [options]\n"
    "Options:\n"
    "  --yy=YYY, -xXXX\n",
    "Usage: test [options]\n"
    "Options:\n"
    "  --yy=YYY -xXXX\n",
    /* 36 */
    "Usage: test [options]\n"
    "Options:\n"
    "  -x XXX\n",
    "Usage: test [options]\n"
    "Options:\n"
    "  --yy=YYY\n",
    "Usage: test [options]\n"
    "Options:\n"
    "  -x XXX, --yy=YYY\n",
    "Usage: test [options]\n"
    "Options:\n"
    "  -x XXX --yy=YYY\n",
    "Usage: test [options]\n"
    "Options:\n"
    "  --yy=YYY, -x XXX\n",
    "Usage: test [options]\n"
    "Options:\n"
    "  --yy=YYY -x XXX\n",
    /* 42 */
    "Usage: test [options]\n"
    "Options:\n"
    "  -xXXX\n",
    "Usage: test [options]\n"
    "Options:\n"
    "  --yy YYY\n",
    "Usage: test [options]\n"
    "Options:\n"
    "  -xXXX, --yy YYY\n",
    "Usage: test [options]\n"
    "Options:\n"
    "  -xXXX --yy YYY\n",
    "Usage: test [options]\n"
    "Options:\n"
    "  --yy YYY, -xXXX\n",
    "Usage: test [options]\n"
    "Options:\n"
    "  --yy YYY -xXXX\n",
    /* 48 */
    "Usage: test [options]\n"
    "Options:\n"
    "  -x XXX\n",
    "Usage: test [options]\n"
    "Options:\n"
    "  --yy YYY\n",
    "Usage: test [options]\n"
    "Options:\n"
    "  -x XXX, --yy YYY\n",
    "Usage: test [options]\n"
    "Options:\n"
    "  -x XXX --yy YYY\n",
    "Usage: test [options]\n"
    "Options:\n"
    "  --yy YYY, -x XXX\n",
    "Usage: test [options]\n"
    "Options:\n"
    "  --yy YYY -x XXX\n",
    /* 54 */
    "Usage: test [options]\n"
    "Options:\n"
    "  -x XXX   FFFFFFFFFFFFFF [default: ZZZ]\n",
    "Usage: test [options]\n"
    "Options:\n"
    "  --yy YYY  FFFFFFFFFFFFFF [default: ZZZ].FFFFFFFFFFF\n",
    "Usage: test [options]\n"
    "Options:\n"
    "  -x XXX, --yy YYY  FFFFFFFFFFFFFF [default: ZZZ].FFFFFFFFFFF\n",
    "Usage: test [options]\n"
    "Options:\n"
    "  -x XXX --yy YYY  FFFFFFFFFFFFFF [default: ZZZ].FFFFFFFFFFF\n",
    "Usage: test [options]\n"
    "Options:\n"
    "  --yy YYY, -x XXX  FFFFFFFFFFFFFF [default: ZZZ].FFFFFFFFFFF\n",
    "Usage: test [options]\n"
    "Options:\n"
    "  --yy YYY -x XXX  FFFFFFFFFFFFFF [default: ZZZ].FFFFFFFFFFF\n",
    /* 60 */
    "Usage: test ARGC ARGV...",
    "Usage: test ARGC [ARGV...]",
    "Usage: test ARGC [ARGV]...",
    "Usage: test cmd ARGC ARGV...",
};

const struct docopt_test tests[] =
{
    { 0, { "-x" },                          { { "-x", 0, 0 } } },
    { 1, { "--yy" },                        { { "--yy", 0, 0 } } },
    { 2, { "-x", "--yy" },                  { { "-x", 0, 0 }, { "--yy", 0, 0 } } },
    { 3, { "-x", "--yy" },                  { { "-x", 0, 0 }, { "--yy", 0, 0 } } },
    { 4, { "-x", "--yy" },                  { { "-x", 0, 0 }, { "--yy", 0, 0 } } },
    { 5, { "-x", "--yy" },                  { { "-x", 0, 0 }, { "--yy", 0, 0 } } },

    { 6, { "-x", "XXX" },                   { { "-x", 0, "XXX" } } },
    { 7, { "--yy" },                        { { "--yy", 0, 0 } } },
    { 8, { "-x", "XXX", "--yy", "XXX" },    { { "-x", 0, "XXX" }, { "--yy", 0, "XXX" } } },
    { 9, { "-x", "XXX", "--yy", "XXX" },    { { "-x", 0, "XXX" }, { "--yy", 0, "XXX" } } },
    { 10, { "-x", "XXX", "--yy", "XXX" },   { { "-x", 0, "XXX" }, { "--yy", 0, "XXX" } } },
    { 11, { "-x", "XXX", "--yy", "XXX" },   { { "-x", 0, "XXX" }, { "--yy", 0, "XXX" } } },

    { 6, { "-xXXX" },                       { { "-x", 0, "XXX" } } },
    { 7, { "--yy" },                        { { "--yy", 0, 0 } } },
    { 8, { "-xXXX", "--yy=XXX" },           { { "-x", 0, "XXX" }, { "--yy", 0, "XXX" } } },
    { 9, { "-xXXX", "--yy=XXX" },           { { "-x", 0, "XXX" }, { "--yy", 0, "XXX" } } },
    { 10, { "-xXXX", "--yy=XXX" },          { { "-x", 0, "XXX" }, { "--yy", 0, "XXX" } } },
    { 11, { "-xXXX", "--yy=XXX" },          { { "-x", 0, "XXX" }, { "--yy", 0, "XXX" } } },

    { 6, { "-xXXX" },                       { { "-x", 0, "XXX" } } },
    { 7, { "--yy" },                        { { "--yy", 0, 0 } } },
    { 8, { "-xXXX", "--yy", "XXX" },        { { "-x", 0, "XXX" }, { "--yy", 0, "XXX" } } },
    { 9, { "-xXXX", "--yy", "XXX" },        { { "-x", 0, "XXX" }, { "--yy", 0, "XXX" } } },
    { 10, { "-xXXX", "--yy", "XXX" },       { { "-x", 0, "XXX" }, { "--yy", 0, "XXX" } } },
    { 11, { "-xXXX", "--yy", "XXX" },       { { "-x", 0, "XXX" }, { "--yy", 0, "XXX" } } },

    { 6, { "-x", "XXX" },                   { { "-x", 0, "XXX" } } },
    { 7, { "--yy" },                        { { "--yy", 0, 0 } } },
    { 8, { "-x", "XXX", "--yy=XXX" },       { { "-x", 0, "XXX" }, { "--yy", 0, "XXX" } } },
    { 9, { "-x", "XXX", "--yy=XXX" },       { { "-x", 0, "XXX" }, { "--yy", 0, "XXX" } } },
    { 10, { "-x", "XXX", "--yy=XXX" },      { { "-x", 0, "XXX" }, { "--yy", 0, "XXX" } } },
    { 11, { "-x", "XXX", "--yy=XXX" },      { { "-x", 0, "XXX" }, { "--yy", 0, "XXX" } } },

    { 6, { "-x", "XXX" },                   { { "-x", 0, "XXX" } } },
    { 7, { "--yy" },                        { { "--yy", 0, 0 } } },
    { 8, { "-x", "XXX", "--yy", "XXX" },    { { "-x", 0, "XXX" }, { "--yy", 0, "XXX" } } },
    { 9, { "-x", "XXX", "--yy", "XXX" },    { { "-x", 0, "XXX" }, { "--yy", 0, "XXX" } } },
    { 10, { "-x", "XXX", "--yy", "XXX" },   { { "-x", 0, "XXX" }, { "--yy", 0, "XXX" } } },
    { 11, { "-x", "XXX", "--yy", "XXX" },   { { "-x", 0, "XXX" }, { "--yy", 0, "XXX" } } },

    { 12, { "-x", "XXX" },                  { { "-x", 0, "XXX" } } },
    { 13, { "--yy" },                       { { "--yy", 0, 0 } } },
    { 14, { "-x", "XXX", "--yy", "XXX" },   { { "-x", 0, "XXX" }, { "--yy", 0, "XXX" } } },
    { 15, { "-x", "XXX", "--yy", "XXX" },   { { "-x", 0, "XXX" }, { "--yy", 0, "XXX" } } },
    { 16, { "-x", "XXX", "--yy", "XXX" },   { { "-x", 0, "XXX" }, { "--yy", 0, "XXX" } } },
    { 17, { "-x", "XXX", "--yy", "XXX" },   { { "-x", 0, "XXX" }, { "--yy", 0, "XXX" } } },

    { 18, { "-x" },                         { { "-x", 0, 0 } } },
    { 19, { "--yy", "XXX" },                { { "--yy", 0, 0 } } },
    { 20, { "-x", "XXX", "--yy", "XXX" },   { { "-x", 0, "XXX" }, { "--yy", 0, "XXX" } } },
    { 21, { "-x", "XXX", "--yy", "XXX" },   { { "-x", 0, "XXX" }, { "--yy", 0, "XXX" } } },
    { 22, { "-x", "XXX", "--yy", "XXX" },   { { "-x", 0, "XXX" }, { "--yy", 0, "XXX" } } },
    { 23, { "-x", "XXX", "--yy", "XXX" },   { { "-x", 0, "XXX" }, { "--yy", 0, "XXX" } } },

    { 24, { "-x" },                         { { "-x", 0, 0 } } },
    { 25, { "--yy", "XXX" },                { { "--yy", 0, "XXX" } } },
    { 26, { "-x", "XXX", "--yy", "XXX" },   { { "-x", 0, "XXX" }, { "--yy", 0, "XXX" } } },
    { 27, { "-x", "XXX", "--yy", "XXX" },   { { "-x", 0, "XXX" }, { "--yy", 0, "XXX" } } },
    { 28, { "-x", "XXX", "--yy", "XXX" },   { { "-x", 0, "XXX" }, { "--yy", 0, "XXX" } } },
    { 29, { "-x", "XXX", "--yy", "XXX" },   { { "-x", 0, "XXX" }, { "--yy", 0, "XXX" } } },

    { 30, { "-x", "XXX" },                  { { "-x", 0, "XXX" } } },
    { 31, { "--yy", "XXX" },                { { "--yy", 0, "XXX" } } },
    { 32, { "-x", "XXX", "--yy", "XXX" },   { { "-x", 0, "XXX" }, { "--yy", 0, "XXX" } } },
    { 33, { "-x", "XXX", "--yy", "XXX" },   { { "-x", 0, "XXX" }, { "--yy", 0, "XXX" } } },
    { 34, { "-x", "XXX", "--yy", "XXX" },   { { "-x", 0, "XXX" }, { "--yy", 0, "XXX" } } },
    { 35, { "-x", "XXX", "--yy", "XXX" },   { { "-x", 0, "XXX" }, { "--yy", 0, "XXX" } } },

    { 36, { "-x", "XXX" },                  { { "-x", 0, "XXX" } } },
    { 37, { "--yy", "XXX" },                { { "--yy", 0, "XXX" } } },
    { 38, { "-x", "XXX", "--yy", "XXX" },   { { "-x", 0, "XXX" }, { "--yy", 0, "XXX" } } },
    { 39, { "-x", "XXX", "--yy", "XXX" },   { { "-x", 0, "XXX" }, { "--yy", 0, "XXX" } } },
    { 40, { "-x", "XXX", "--yy", "XXX" },   { { "-x", 0, "XXX" }, { "--yy", 0, "XXX" } } },
    { 41, { "-x", "XXX", "--yy", "XXX" },   { { "-x", 0, "XXX" }, { "--yy", 0, "XXX" } } },

    { 42, { "-x", "XXX" },                  { { "-x", 0, "XXX" } } },
    { 43, { "--yy", "XXX" },                { { "--yy", 0, "XXX" } } },
    { 44, { "-x", "XXX", "--yy", "XXX" },   { { "-x", 0, "XXX" }, { "--yy", 0, "XXX" } } },
    { 45, { "-x", "XXX", "--yy", "XXX" },   { { "-x", 0, "XXX" }, { "--yy", 0, "XXX" } } },
    { 46, { "-x", "XXX", "--yy", "XXX" },   { { "-x", 0, "XXX" }, { "--yy", 0, "XXX" } } },
    { 47, { "-x", "XXX", "--yy", "XXX" },   { { "-x", 0, "XXX" }, { "--yy", 0, "XXX" } } },

    { 48, { "-x", "XXX" },                  { { "-x", 0, "XXX" } } },
    { 49, { "--yy", "XXX" },                { { "--yy", 0, "XXX" } } },
    { 50, { "-x", "XXX", "--yy", "XXX" },   { { "-x", 0, "XXX" }, { "--yy", 0, "XXX" } } },
    { 51, { "-x", "XXX", "--yy", "XXX" },   { { "-x", 0, "XXX" }, { "--yy", 0, "XXX" } } },
    { 52, { "-x", "XXX", "--yy", "XXX" },   { { "-x", 0, "XXX" }, { "--yy", 0, "XXX" } } },
    { 53, { "-x", "XXX", "--yy", "XXX" },   { { "-x", 0, "XXX" }, { "--yy", 0, "XXX" } } },

    { 54, { },                              { { "-x", 0, "ZZZ" } } },
    { 55, { },                              { { "--yy", 0, "ZZZ" } } },
    { 56, { },                              { { "-x", 0, "ZZZ" }, { "--yy", 0, "ZZZ" } } },
    { 57, { },                              { { "-x", 0, "ZZZ" }, { "--yy", 0, "ZZZ" } } },
    { 58, { },                              { { "-x", 0, "ZZZ" }, { "--yy", 0, "ZZZ" } } },
    { 59, { },                              { { "-x", 0, "ZZZ" }, { "--yy", 0, "ZZZ" } } },

    { 60, { "A0", "A1", "A2", "A3" },       { { "ARGC", 0, "A0" }, { "ARGV", 0, "A1" }, { "ARGV", 1, "A2" }, { "ARGV", 2, "A3" } } },
    { 61, { "A0" },                         { { "ARGC", 0, "A0" } } },
    { 62, { "A0" },                         { { "ARGC", 0, "A0" } } },
    { 63, { "cmd", "A0" },                  { { "cmd", 0, 0 }, { "ARGC", 0, "A0" } } },
};

int check_test(docopt_t doc, int test);

int main(int argc, char **argv)
{
    const char docstr[] =
    "Usage:\n"
    "  test self-test\n"
    "  test one N\n";

    docopt_t doc        = docopt(docstr, argc-1, argv+1, NULL, 0);
    unsigned long low   = 0;
    unsigned long high  = sizeof(tests)/sizeof(tests[0]);
    if (doc && !docopt_error(doc))
    {
        if (docopt_get(doc, "one", 0))
        {
            low         = strtoul(docopt_get(doc, "N", 0)->fst, 0, 0);
            high        = low + 1;
        }
    }
    else
    {
        return 1;
    }
    docopt_free(doc);

    for (int i = low, e = high; i < e; ++i)
    {
        int argc        = 0;
        for (int j = 0; tests[i].args[j]; ++j, ++argc)
            ;
        docopt_t doc    = docopt(usages[tests[i].usage], argc, (char**)tests[i].args, NULL, DOCOPT_NO_HELP);
        if (!doc)
        {
            fprintf(stdout, "Failed to parse (%d):\n====\n%s\n====\n", i, usages[tests[i].usage]);
            return 1;
        }
        else if (docopt_error(doc))
        {
            assert(docopt_error(doc) == DOCOPT_PRINTED_HELP);
            if (check_test(doc, i)) return 1;
        }
        else
        {
            if (check_test(doc, i)) return 1;
        }
        docopt_free(doc);
    }

    return 0;
}

int check_test(docopt_t doc, int i)
{
    for (int j = 0; tests[i].checks[j].key; ++j)
    {
        struct docopt_str* get  = docopt_get(doc, tests[i].checks[j].key, tests[i].checks[j].index);
        if (!get)
        {
            fprintf(stdout, "(%d): %s\n", i, usages[tests[i].usage]);
            fprintf(stdout, "Failed to get (%d.%d.%d) %s\n", i, j, tests[i].usage, tests[i].checks[j].key);
            return 1;
        }
        else
        {
            if (tests[i].checks[j].value)
            {
                if ((strlen(tests[i].checks[j].value) != docopt_str_len(get)) ||
                    strncmp(tests[i].checks[j].value, get->fst, docopt_str_len(get)))
                {
                    fprintf(stdout, "(%d): %s\n", i, usages[tests[i].usage]);
                    fprintf(stdout, "Mismatch (%d.%d.%d) %s wants='%s' gots='%s'\n", i, j, tests[i].usage, tests[i].checks[j].key, tests[i].checks[j].value, get->fst);
                    return 1;
                }
            }
        }
    }
    return 0;
}
