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
#define DOCOPT_DEBUG
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
    /* 64 */
    "Usage: test <arg-c> <arg-v>...",
    "Usage: test <arg-c> [<arg-v>...]",
    "Usage: test <arg-c> [<arg-v>]...",
    "Usage: test cmd <arg-c> <arg-v>...",
    /* 68 */
    "Usage: arguments_example.py [-vqrh] [FILE] ...\n"
    "          arguments_example.py (--left | --right) CORRECTION FILE\n"
    "\n"
    "Process FILE and optionally apply correction to either left-hand side or\n"
    "right-hand side.\n"
    "\n"
    "Arguments:\n"
    "  FILE        optional input file\n"
    "  CORRECTION  correction angle, needs FILE, --left or --right to be present\n"
    "\n"
    "Options:\n"
    "  -h --help\n"
    "  -v       verbose mode\n"
    "  -q       quiet mode\n"
    "  -r       make report\n"
    "  --left   use left-hand side\n"
    "  --right  use right-hand side\n",
    /* 69 */
    "Not a serious example.\n"
    "\n"
    "Usage:\n"
    "  calculator_example.py <value> ( ( + | - | * | / ) <value> )...\n"
    "  calculator_example.py <function> <value> [( , <value> )]...\n"
    "  calculator_example.py (-h | --help)\n"
    "\n"
    "Examples:\n"
    "  calculator_example.py 1 + 2 + 3 + 4 + 5\n"
    "  calculator_example.py 1 + 2 '*' 3 / 4 - 5    # note quotes around '*'\n"
    "  calculator_example.py sum 10 , 20 , 30 , 40\n"
    "\n"
    "Options:\n"
    "  -h, --help\n",
    /* 70 */
    "Usage: counted_example.py --help\n"
    "       counted_example.py -v...\n"
    "       counted_example.py go [go]\n"
    "       counted_example.py (--path=<path>)...\n"
    "       counted_example.py <file> <file>\n"
    "\n"
    "Try: counted_example.py -vvvvvvvvvv\n"
    "     counted_example.py go go\n"
    "     counted_example.py --path ./here --path ./there\n"
    "     counted_example.py this.txt that.txt\n",
    /* 71 */
    "This example uses docopt with the built in cmd module to demonstrate an\n"
    "interactive command application.\n"
    "\n"
    "Usage:\n"
    "    my_program tcp <host> <port> [--timeout=<seconds>]\n"
    "    my_program serial <port> [--baud=<n>] [--timeout=<seconds>]\n"
    "    my_program (-i | --interactive)\n"
    "    my_program (-h | --help | --version)\n"
    "\n"
    "Options:\n"
    "    -i, --interactive  Interactive Mode\n"
    "    -h, --help  Show this screen and exit.\n"
    "    --baud=<n>  Baudrate [default: 9600]\n",
    /* 72 */
    "Usage:\n"
    "  naval_fate.py ship new <name>...\n"
    "  naval_fate.py ship <name> move <x> <y> [--speed=<kn>]\n"
    "  naval_fate.py ship shoot <x> <y>\n"
    "  naval_fate.py mine (set|remove) <x> <y> [--moored|--drifting]\n"
    "  naval_fate.py -h | --help\n"
    "  naval_fate.py --version\n"
    "\n"
    "Options:\n"
    "  -h --help     Show this screen.\n"
    "  --version     Show version.\n"
    "  --speed=<kn>  Speed in knots [default: 10].\n"
    "  --moored      Moored (anchored) mine.\n"
    "  --drifting    Drifting mine.\n",
    /* 73 */
    "Usage: odd_even_example.py [-h | --help] (ODD EVEN)...\n"
    "\n"
    "Example, try:\n"
    "  odd_even_example.py 1 2 3 4\n"
    "\n"
    "Options:\n"
    "  -h, --help\n",
    /* 74 */
    "Example of program with many options using docopt.\n"
    "\n"
    "Usage:\n"
    "  options_example.py [-hvqrf NAME] [--exclude=PATTERNS]\n"
    "                     [--select=ERRORS | --ignore=ERRORS] [--show-source]\n"
    "                     [--statistics] [--count] [--benchmark] PATH...\n"
    "  options_example.py (--doctest | --testsuite=DIR)\n"
    "  options_example.py --version\n"
    "\n"
    "Arguments:\n"
    "  PATH  destination path\n"
    "\n"
    "Options:\n"
    "  -h --help            show this help message and exit\n"
    "  --version            show version and exit\n"
    "  -v --verbose         print status messages\n"
    "  -q --quiet           report only file names\n"
    "  -r --repeat          show all occurrences of the same error\n"
    "  --exclude=PATTERNS   exclude files or directories which match these comma\n"
    "                       separated patterns [default: .svn,CVS,.bzr,.hg,.git]\n"
    "  -f NAME --file=NAME  when parsing directories, only check filenames matching\n"
    "                       these comma separated patterns [default: *.py]\n"
    "  --select=ERRORS      select errors and warnings (e.g. E,W6)\n"
    "  --ignore=ERRORS      skip errors and warnings (e.g. E4,W)\n"
    "  --show-source        show source code for each error\n"
    "  --statistics         count errors and warnings\n"
    "  --count              print total number of errors and warnings to standard\n"
    "                       error and set exit code to 1 if total is not null\n"
    "  --benchmark          measure processing speed\n"
    "  --testsuite=DIR      run regression tests from dir\n"
    "  --doctest            run doctest on myself\n",
    /* 75 */
    "Usage: test ARG [-- OPTARG...]",
    /* 76 */
    "Usage: test [<arg0> <arg1> <arg2>]",
    /* 77 */
    "Usage: test ( cmda <sw0> <sw1>\n"
    "            | cmdb <sw0> <sw1>\n"
    "            | cmdc <a-0> <a-1> <a-2> <a-3>)\n"
    "            [cmdd A B | cmde N | cmdf | cmdg]",
};

const struct docopt_test tests[] =
{
    { 0, { "-x" },                          { { "-x", 0, 0 } } },
    { 1, { "--yy" },                        { { "--yy", 0, 0 } } },
    { 2, { "-x", "--yy" },                  { { "-x", 0, 0 }, { "--yy", 0, 0 } } },
    { 3, { "-x", "--yy" },                  { { "-x", 0, 0 }, { "--yy", 0, 0 } } },
    { 4, { "-x", "--yy" },                  { { "-x", 0, 0 }, { "--yy", 0, 0 } } },
    { 5, { "-x", "--yy" },                  { { "-x", 0, 0 }, { "--yy", 0, 0 } } },
    /* #6 */
    { 6, { "-x", "XXX" },                   { { "-x", 0, "XXX" } } },
    { 7, { "--yy" },                        { { "--yy", 0, 0 } } },
    { 8, { "-x", "XXX", "--yy", "XXX" },    { { "-x", 0, "XXX" }, { "--yy", 0, "XXX" } } },
    { 9, { "-x", "XXX", "--yy", "XXX" },    { { "-x", 0, "XXX" }, { "--yy", 0, "XXX" } } },
    { 10, { "-x", "XXX", "--yy", "XXX" },   { { "-x", 0, "XXX" }, { "--yy", 0, "XXX" } } },
    { 11, { "-x", "XXX", "--yy", "XXX" },   { { "-x", 0, "XXX" }, { "--yy", 0, "XXX" } } },
    /* #12 */
    { 6, { "-xXXX" },                       { { "-x", 0, "XXX" } } },
    { 7, { "--yy" },                        { { "--yy", 0, 0 } } },
    { 8, { "-xXXX", "--yy=XXX" },           { { "-x", 0, "XXX" }, { "--yy", 0, "XXX" } } },
    { 9, { "-xXXX", "--yy=XXX" },           { { "-x", 0, "XXX" }, { "--yy", 0, "XXX" } } },
    { 10, { "-xXXX", "--yy=XXX" },          { { "-x", 0, "XXX" }, { "--yy", 0, "XXX" } } },
    { 11, { "-xXXX", "--yy=XXX" },          { { "-x", 0, "XXX" }, { "--yy", 0, "XXX" } } },
    /* #18 */
    { 6, { "-xXXX" },                       { { "-x", 0, "XXX" } } },
    { 7, { "--yy" },                        { { "--yy", 0, 0 } } },
    { 8, { "-xXXX", "--yy", "XXX" },        { { "-x", 0, "XXX" }, { "--yy", 0, "XXX" } } },
    { 9, { "-xXXX", "--yy", "XXX" },        { { "-x", 0, "XXX" }, { "--yy", 0, "XXX" } } },
    { 10, { "-xXXX", "--yy", "XXX" },       { { "-x", 0, "XXX" }, { "--yy", 0, "XXX" } } },
    { 11, { "-xXXX", "--yy", "XXX" },       { { "-x", 0, "XXX" }, { "--yy", 0, "XXX" } } },
    /* #24 */
    { 6, { "-x", "XXX" },                   { { "-x", 0, "XXX" } } },
    { 7, { "--yy" },                        { { "--yy", 0, 0 } } },
    { 8, { "-x", "XXX", "--yy=XXX" },       { { "-x", 0, "XXX" }, { "--yy", 0, "XXX" } } },
    { 9, { "-x", "XXX", "--yy=XXX" },       { { "-x", 0, "XXX" }, { "--yy", 0, "XXX" } } },
    { 10, { "-x", "XXX", "--yy=XXX" },      { { "-x", 0, "XXX" }, { "--yy", 0, "XXX" } } },
    { 11, { "-x", "XXX", "--yy=XXX" },      { { "-x", 0, "XXX" }, { "--yy", 0, "XXX" } } },
    /* #30 */
    { 6, { "-x", "XXX" },                   { { "-x", 0, "XXX" } } },
    { 7, { "--yy" },                        { { "--yy", 0, 0 } } },
    { 8, { "-x", "XXX", "--yy", "XXX" },    { { "-x", 0, "XXX" }, { "--yy", 0, "XXX" } } },
    { 9, { "-x", "XXX", "--yy", "XXX" },    { { "-x", 0, "XXX" }, { "--yy", 0, "XXX" } } },
    { 10, { "-x", "XXX", "--yy", "XXX" },   { { "-x", 0, "XXX" }, { "--yy", 0, "XXX" } } },
    { 11, { "-x", "XXX", "--yy", "XXX" },   { { "-x", 0, "XXX" }, { "--yy", 0, "XXX" } } },
    /* #36 */
    { 12, { "-x", "XXX" },                  { { "-x", 0, "XXX" } } },
    { 13, { "--yy" },                       { { "--yy", 0, 0 } } },
    { 14, { "-x", "XXX", "--yy", "XXX" },   { { "-x", 0, "XXX" }, { "--yy", 0, "XXX" } } },
    { 15, { "-x", "XXX", "--yy", "XXX" },   { { "-x", 0, "XXX" }, { "--yy", 0, "XXX" } } },
    { 16, { "-x", "XXX", "--yy", "XXX" },   { { "-x", 0, "XXX" }, { "--yy", 0, "XXX" } } },
    { 17, { "-x", "XXX", "--yy", "XXX" },   { { "-x", 0, "XXX" }, { "--yy", 0, "XXX" } } },
    /* #42 */
    { 18, { "-x" },                         { { "-x", 0, 0 } } },
    { 19, { "--yy", "XXX" },                { { "--yy", 0, 0 } } },
    { 20, { "-x", "XXX", "--yy", "XXX" },   { { "-x", 0, "XXX" }, { "--yy", 0, "XXX" } } },
    { 21, { "-x", "XXX", "--yy", "XXX" },   { { "-x", 0, "XXX" }, { "--yy", 0, "XXX" } } },
    { 22, { "-x", "XXX", "--yy", "XXX" },   { { "-x", 0, "XXX" }, { "--yy", 0, "XXX" } } },
    { 23, { "-x", "XXX", "--yy", "XXX" },   { { "-x", 0, "XXX" }, { "--yy", 0, "XXX" } } },
    /* #48 */
    { 24, { "-x" },                         { { "-x", 0, 0 } } },
    { 25, { "--yy", "XXX" },                { { "--yy", 0, "XXX" } } },
    { 26, { "-x", "XXX", "--yy", "XXX" },   { { "-x", 0, "XXX" }, { "--yy", 0, "XXX" } } },
    { 27, { "-x", "XXX", "--yy", "XXX" },   { { "-x", 0, "XXX" }, { "--yy", 0, "XXX" } } },
    { 28, { "-x", "XXX", "--yy", "XXX" },   { { "-x", 0, "XXX" }, { "--yy", 0, "XXX" } } },
    { 29, { "-x", "XXX", "--yy", "XXX" },   { { "-x", 0, "XXX" }, { "--yy", 0, "XXX" } } },
    /* #54 */
    { 30, { "-x", "XXX" },                  { { "-x", 0, "XXX" } } },
    { 31, { "--yy", "XXX" },                { { "--yy", 0, "XXX" } } },
    { 32, { "-x", "XXX", "--yy", "XXX" },   { { "-x", 0, "XXX" }, { "--yy", 0, "XXX" } } },
    { 33, { "-x", "XXX", "--yy", "XXX" },   { { "-x", 0, "XXX" }, { "--yy", 0, "XXX" } } },
    { 34, { "-x", "XXX", "--yy", "XXX" },   { { "-x", 0, "XXX" }, { "--yy", 0, "XXX" } } },
    { 35, { "-x", "XXX", "--yy", "XXX" },   { { "-x", 0, "XXX" }, { "--yy", 0, "XXX" } } },
    /* #60 */
    { 36, { "-x", "XXX" },                  { { "-x", 0, "XXX" } } },
    { 37, { "--yy", "XXX" },                { { "--yy", 0, "XXX" } } },
    { 38, { "-x", "XXX", "--yy", "XXX" },   { { "-x", 0, "XXX" }, { "--yy", 0, "XXX" } } },
    { 39, { "-x", "XXX", "--yy", "XXX" },   { { "-x", 0, "XXX" }, { "--yy", 0, "XXX" } } },
    { 40, { "-x", "XXX", "--yy", "XXX" },   { { "-x", 0, "XXX" }, { "--yy", 0, "XXX" } } },
    { 41, { "-x", "XXX", "--yy", "XXX" },   { { "-x", 0, "XXX" }, { "--yy", 0, "XXX" } } },
    /* #66 */
    { 42, { "-x", "XXX" },                  { { "-x", 0, "XXX" } } },
    { 43, { "--yy", "XXX" },                { { "--yy", 0, "XXX" } } },
    { 44, { "-x", "XXX", "--yy", "XXX" },   { { "-x", 0, "XXX" }, { "--yy", 0, "XXX" } } },
    { 45, { "-x", "XXX", "--yy", "XXX" },   { { "-x", 0, "XXX" }, { "--yy", 0, "XXX" } } },
    { 46, { "-x", "XXX", "--yy", "XXX" },   { { "-x", 0, "XXX" }, { "--yy", 0, "XXX" } } },
    { 47, { "-x", "XXX", "--yy", "XXX" },   { { "-x", 0, "XXX" }, { "--yy", 0, "XXX" } } },
    /* #72 */
    { 48, { "-x", "XXX" },                  { { "-x", 0, "XXX" } } },
    { 49, { "--yy", "XXX" },                { { "--yy", 0, "XXX" } } },
    { 50, { "-x", "XXX", "--yy", "XXX" },   { { "-x", 0, "XXX" }, { "--yy", 0, "XXX" } } },
    { 51, { "-x", "XXX", "--yy", "XXX" },   { { "-x", 0, "XXX" }, { "--yy", 0, "XXX" } } },
    { 52, { "-x", "XXX", "--yy", "XXX" },   { { "-x", 0, "XXX" }, { "--yy", 0, "XXX" } } },
    { 53, { "-x", "XXX", "--yy", "XXX" },   { { "-x", 0, "XXX" }, { "--yy", 0, "XXX" } } },
    /* #78 */
    { 54, { },                              { { "-x", 0, "ZZZ" } } },
    { 55, { },                              { { "--yy", 0, "ZZZ" } } },
    { 56, { },                              { { "-x", 0, "ZZZ" }, { "--yy", 0, "ZZZ" } } },
    { 57, { },                              { { "-x", 0, "ZZZ" }, { "--yy", 0, "ZZZ" } } },
    { 58, { },                              { { "-x", 0, "ZZZ" }, { "--yy", 0, "ZZZ" } } },
    { 59, { },                              { { "-x", 0, "ZZZ" }, { "--yy", 0, "ZZZ" } } },

    { 60, { "A0", "A1", "A2", "A3" },       { { "ARGC", 0, "A0" }, { "ARGV", 0, "A1" }, { "ARGV", 1, "A2" }, { "ARGV", 2, "A3" } } },
    { 61, { "A0" },                         { { "ARGC", 0, "A0" } } },
    { 62, { "A0" },                         { { "ARGC", 0, "A0" } } },
    { 63, { "cmd", "A0", "A1" },            { { "cmd", 0, 0 }, { "ARGC", 0, "A0" }, { "ARGV", 0, "A1" } } },

    { 64, { "A0", "A1", "A2", "A3" },       { { "<arg-c>", 0, "A0" }, { "<arg-v>", 0, "A1" }, { "<arg-v>", 1, "A2" }, { "<arg-v>", 2, "A3" } } },
    { 65, { "A0" },                         { { "<arg-c>", 0, "A0" } } },
    { 66, { "A0" },                         { { "<arg-c>", 0, "A0" } } },
    { 67, { "cmd", "A0", "A1" },            { { "cmd", 0, 0 }, { "<arg-c>", 0, "A0" }, { "<arg-v>", 0, "A1" } } },

    { 68, { "--help" },                     { } },
    { 68, { "-v", "FOO" },                  { { "-v", 0, 0 }, { "FILE", 0, "FOO" } } },
    { 68, { "-q", "FOO" },                  { { "-q", 0, 0 }, { "FILE", 0, "FOO" } } },
    { 68, { "-r", "FOO" },                  { { "-r", 0, 0 }, { "FILE", 0, "FOO" } } },
    { 68, { "--left", "FOO", "BAR" },       { { "--left", 0, 0 }, { "CORRECTION", 0, "FOO" }, { "FILE", 0, "BAR" } } },
    { 68, { "--right", "FOO", "BAR" },      { { "--right", 0, 0 }, { "CORRECTION", 0, "FOO" }, { "FILE", 0, "BAR" } } },

    { 69, { "--help" },                     { } },
    { 69, { "1", "+", "2" },                { { "<value>", 0, "1" }, { "<value>", 1, "2" } } },
    { 69, { "3", "+", "4", "-", "5" },      { { "<value>", 0, "3" }, { "<value>", 1, "4" }, { "<value>", 2, "5" } } },
    { 69, { "sum", "10", ",", "20" },       { { "<function>", 0, "sum" }, { "<value>", 0, "10" }, { "<value>", 1, "20" } } },

    { 70, { "--help" },                     { } },
    { 70, { "-vvvvv"},                      { { "-v", 0, 0 }, { "-v", 1, 0 }, { "-v", 2, 0 }, { "-v", 3, 0 }, { "-v", 4, 0 } } },
    { 70, { "go", "go" },                   { { "go", 0, 0 }, { "go", 1, 0 } } },
    { 70, { "--path", "./here", "--path", "./there"}, { { "--path", 0, "./here"}, { "--path", 1, "./there"}, } },
    { 70, { "this.txt", "that.txt" },       { { "<file>", 0, "this.txt" }, { "<file>", 1, "that.txt" } } },

    { 71, { "--help" },                     { } },
    { 71, { "tcp", "google.com", "8080" },  { { "tcp", 0, 0}, { "<host>", 0, "google.com" }, { "<port>", 0, "8080" } } },
    { 71, { "tcp", "google.com", "8080", "--timeout=1000ms" },  { { "tcp", 0, 0}, { "<host>", 0, "google.com" }, { "<port>", 0, "8080" }, { "--timeout", 0, "1000ms" } } },
    { 71, { "serial", "8080" },             { { "serial", 0, 0}, { "<port>", 0, "8080" } } },
    { 71, { "serial", "8080" },             { { "serial", 0, 0}, { "<port>", 0, "8080" }, { "--baud", 0, "9600" } } },
    { 71, { "serial", "8080", "--baud", "1200" }, { { "serial", 0, 0}, { "<port>", 0, "8080" }, { "--baud", 0, "1200" } } },
    { 71, { "serial", "8080", "--timeout", "5000ms" }, { { "serial", 0, 0}, { "<port>", 0, "8080" }, { "--baud", 0, "9600" }, { "--timeout", 0, "5000ms" } } },
    { 71, { "--interactive" },              { { "--interactive", 0, 0 } } },
    { 71, { "--interactive" },              { { "-i", 0, 0 } } },
    { 71, { "-i" },                         { { "--interactive", 0, 0 } } },
    { 71, { "-i" },                         { { "-i", 0, 0 } } },

    { 72, { "--help" },                     { } },
    { 72, { "ship", "new", "USS Enterprise", "USS Kennedy" }, { { "ship", 0, 0 }, { "new", 0, 0 }, { "<name>", 0, "USS Enterprise" }, { "<name>", 1, "USS Kennedy" } } },
    { 72, { "ship", "USS Enterprise", "move", "10", "20" }, { { "ship", 0, 0 }, { "<name>", 0, "USS Enterprise" } } },
    { 72, { "ship", "USS Enterprise", "move", "10", "20" }, { { "ship", 0, 0 }, { "<name>", 0, "USS Enterprise" }, { "--speed", 0, "10" } } },
    { 72, { "ship", "USS Enterprise", "move", "10", "20", "--speed=11.3" }, { { "ship", 0, 0 }, { "<name>", 0, "USS Enterprise" }, { "--speed", 0, "11.3" } } },
    { 72, { "ship", "shoot", "3", "4" },    { { "ship", 0, 0 }, { "shoot", 0, 0 }, { "<x>", 0, "3" }, { "<y>", 0, "4" } } },
    { 72, { "mine", "set", "10", "12" },    { { "mine", 0, 0}, { "set", 0, 0 }, { "<x>", 0, "10" }, { "<y>", 0, "12" } } },
    { 72, { "mine", "set", "10", "12", "--moored" }, { { "mine", 0, 0}, { "set", 0, 0 }, { "<x>", 0, "10" }, { "<y>", 0, "12" }, { "--moored", 0, 0 } } },
    { 72, { "mine", "set", "10", "12", "--drifting" }, { { "mine", 0, 0}, { "set", 0, 0 }, { "<x>", 0, "10" }, { "<y>", 0, "12" }, { "--drifting", 0, 0 } } },
    { 72, { "mine", "remove", "10", "12" },    { { "mine", 0, 0}, { "remove", 0, 0 }, { "<x>", 0, "10" }, { "<y>", 0, "12" } } },
    { 72, { "mine", "remove", "10", "12", "--moored" }, { { "mine", 0, 0}, { "remove", 0, 0 }, { "<x>", 0, "10" }, { "<y>", 0, "12" }, { "--moored", 0, 0 } } },
    { 72, { "mine", "remove", "10", "12", "--drifting" }, { { "mine", 0, 0}, { "remove", 0, 0 }, { "<x>", 0, "10" }, { "<y>", 0, "12" }, { "--drifting", 0, 0 } } },

    { 73, { "--help" },                     { } },
    { 73, { "1", "2", "3", "4" },           { { "ODD", 0, "1" }, { "EVEN", 0, "2" }, { "ODD", 1, "3" }, { "EVEN", 1, "4" } } },

    { 74, { "--help" },                     { } },
    { 74, { "-fFOO", "HERE", "THERE" },     { { "-f", 0, "FOO" }, { "PATH", 0, "HERE" }, { "PATH", 1, "THERE" } } },
    { 74, { "--doctest" },                  { { "--doctest", 0, 0 }, { "-f", 0, "*.py" }, { "--file", 0, "*.py"} } },

    { 75, { "--help" },                     { } },
    { 75, { "A", "--", "C", "D", "E" },     { { "ARG", 0, "A" }, { "OPTARG", 0, "C" }, { "OPTARG", 1, "D" }, { "OPTARG", 2, "E" } } },
    { 75, { "A", "--", "--C", "--D", "--E" }, { { "ARG", 0, "A" }, { "OPTARG", 0, "--C" }, { "OPTARG", 1, "--D" }, { "OPTARG", 2, "--E" } } },

    { 76, { "--help" },                     { } },
    { 76, { "A" },                          { { "<arg0>", 0, "A" } } },
    { 76, { "A", "B" },                     { { "<arg0>", 0, "A" }, { "<arg1>", 0, "B" } } },
    { 76, { "A", "B", "C" },                { { "<arg0>", 0, "A" }, { "<arg1>", 0, "B" }, { "<arg2>", 0, "C" } } },

    { 77, { "--help" },                     { } },
    { 77, { "cmda", "0", "1" },             { { "cmda", 0, 0 }, { "<sw0>", 0, "0" }, { "<sw1>", 0, "1" } } },
    { 77, { "cmda", "0", "1", "cmdd", "A", "B" }, { { "cmda", 0, 0 }, { "<sw0>", 0, "0" }, { "<sw1>", 0, "1" }, { "cmdd", 0, 0 }, { "A", 0, "A" }, { "B", 0, "B" } } },
    { 77, { "cmda", "0", "1", "cmde", "X" }, { { "cmda", 0, 0 }, { "<sw0>", 0, "0" }, { "<sw1>", 0, "1" }, { "cmde", 0, 0 }, { "N", 0, "X" } } },
    { 77, { "cmdc", "a", "b", "c", "d", "cmdf" }, { { "cmdc", 0, 0 }, { "<a-0>", 0, "a" }, { "<a-1>", 0, "b" }, { "<a-2>", 0, "c" }, { "<a-3>", 0, "d" }, { "cmdf", 0, 0 } } },
};

int check_test(docopt_t doc, int test);

int main(int argc, char **argv)
{
    const char docstr[] =
    "Usage:\n"
    "  test self-test\n"
    "  test one N\n";

    docopt_t doc        = docopt(docstr, argc-1, argv+1, 0, DOCOPT_VERSION);
    unsigned long low   = 0;
    unsigned long high  = sizeof(tests)/sizeof(tests[0]);
    if (doc && !docopt_error(doc))
    {
        if (docopt_get(doc, "one", 0))
        {
            low         = strtoul(docopt_get(doc, "N", 0)->fst, 0, 0);
            high        = low + 1;
            fprintf(stdout, "ARGS\n");
            for (int j = 0; tests[low].args[j]; ++j)
                fprintf(stdout, "    %s\n", tests[low].args[j]);
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
        docopt_t doc    = docopt(usages[tests[i].usage], argc, (char**)tests[i].args, DOCOPT_NO_HELP, DOCOPT_VERSION);
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
