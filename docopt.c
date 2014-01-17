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

int main(int argc, char **argv)
{
    const char msg[] =
    "Usage:\n"
    "    docopt [options] DOCSTR ARGC ARGV...\n"
    "    docopt (-h | --help | --version)\n"
    "    docopt --pptions=<DIR>"
    "\n"
    "Options:\n"
    "    -p, --print FILE   The file to print [default: driver@].\n"
    ;
    int i = 0;

    docopt_t opts   = docopt(msg, --argc, ++argv, 0, DOCOPT_HELP);
    if (opts && !docopt_error(opts))
    {
        fprintf(stdout, "Has help? %d\n", !!docopt_get(opts, "--help", 0));
        fprintf(stdout, "Has docstr? %s\n", docopt_get(opts, "DOCSTR", 0) ? docopt_get(opts, "DOCSTR", 0)->fst : "no");
        fprintf(stdout, "Has argc? %s\n", docopt_get(opts, "ARGC", 0) ? docopt_get(opts, "ARGC", 0)->fst : "no");
        while (docopt_get(opts, "ARGV", i))
        {
            fprintf(stdout, "Has argv? %s\n", docopt_get(opts, "ARGV", i)->fst);
            ++i;
        }
        fprintf(stdout, "Has doc? %d\n", !!docopt_get(opts, "doc", 0));
        fprintf(stdout, "Has STR? %s\n", docopt_get(opts, "STR", 0) ? docopt_get(opts, "STR", 0)->fst : "no");
        fprintf(stdout, "Has version? %s\n", docopt_get(opts, "--version", 0) ? docopt_get(opts, "--version", 0)->fst : "no");
        fprintf(stdout, "Has -l? %s\n", docopt_get(opts, "-l", 0) ? docopt_get(opts, "-l", 0)->fst : "no");
        fprintf(stdout, "Has -f? %s\n", docopt_get(opts, "-f", 0) ? docopt_get(opts, "-f", 0)->fst : "no");
        fprintf(stdout, "Has -j? %s\n", docopt_get(opts, "-j", 0) ? docopt_get(opts, "-j", 0)->fst : "no");
        fprintf(stdout, "Has -k? %s\n", docopt_get(opts, "-k", 0) ? docopt_get(opts, "-k", 0)->fst : "no");
        fprintf(stdout, "Has -o? %s\n", docopt_get(opts, "-o", 0) ? docopt_get(opts, "-o", 0)->fst : "no");
        fprintf(stdout, "Has -p? %s\n", docopt_get(opts, "-p", 0) ? docopt_get(opts, "-p", 0)->fst : "no");
    }
    docopt_free(opts);


    return 0;
}
