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

#ifndef DOCOPT_H
#define DOCOPT_H

#define DOCOPT_VERSION              0x00000100
#define DOCOPT_OK                   0
#define DOCOPT_BAD_DEFAULTS         1
#define DOCOPT_BAD_USAGE_NAME       2
#define DOCOPT_BAD_USAGE_STATEMENT  3
#define DOCOPT_BAD_ARGED_SHORTS     4
#define DOCOPT_PRINTED_HELP         5
#define DOCOPT_BAD_ARG_ALLOC        6
#define DOCOPT_BAD_VERSION          7

#define DOCOPT_NO_HELP              0x1
#define DOCOPT_HELP                 0x0
#define DOCOPT_NO_OPTIONS_FIRST     0x2
#define DOCOPT_OPTIONS_FIRST        0x0

#define PRdocS                      "%.*s"
#define DOCOPT_PR(str)              docopt_str_len((str)), (str)->fst

typedef void* docopt_t;
typedef struct docopt_str { const char* fst; const char* lst; } docopt_str_t;

docopt_t docopt(const char* doc, int argc, char** argv, unsigned int flags, unsigned int version);
docopt_str_t* docopt_get(docopt_t, const char*, int);
int docopt_error(docopt_t);
void docopt_free(docopt_t);

#endif/*DOCOPT_H*/

/*=============================================================================*/

#ifdef  DOCOPT_C

#include <assert.h>
#include <ctype.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DOCOPT_TYPE_TBL(X) X(BAD) X(USAGE_SECTION) X(USAGE_STATEMENT) X(OPTION_SECTION) X(OPTION_STATEMENT) X(LONG) X(SHORT) X(OPTREF) X(PNAME) X(COMMAND) X(ARGUMENT) X(SEQ) X(EXPR) X(MAX)
enum DOCOPT_TYPE
{
#define DOCOPT_TYPE_ENUM(N) DOCOPT_TYPE_##N,
DOCOPT_TYPE_TBL(DOCOPT_TYPE_ENUM)
};
static const char* DOCOPT_TYPE_NAMES[] =
{
#define DOCOPT_TYPE_NAME(N) #N,
DOCOPT_TYPE_TBL(DOCOPT_TYPE_NAME)
};

#define DOCOPT_NULL_FILE                    "/dev/null"
#define DOCOPT_RE_PNAME                     "([[:space:]]*([^[:space:]]*))"
#define DOCOPT_RE_ALL_SHORTS_CHARS          "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ\\@\\$\\?"
#define DOCOPT_RE_UPPER_ARGUMENT            "([A-Z]+)"
#define DOCOPT_RE_ARGUMENT                  "(" DOCOPT_RE_UPPER_ARGUMENT "|(\\<[^\\>]+\\>))"
#define DOCOPT_RE_DEFAULT                   "(\\[default:[[:space:]]*([^]]*)\\])"
#define DOCOPT_RE_OPTSHORT                  "(\\-([" DOCOPT_RE_ALL_SHORTS_CHARS "])([ ]?(" DOCOPT_RE_UPPER_ARGUMENT "))?)"
#define DOCOPT_RE_LONG                      "(\\-\\-([a-zA-Z][a-zA-Z\\-\\_]*)([ =](" DOCOPT_RE_ARGUMENT "))?)"
#define DOCOPT_RE_REPEAT                    "(\\.\\.\\.)"
#define DOCOPT_RE_RESERVED                  "([\\[\\]\\(\\)\\|])"
#define DOCOPT_RE_USAGE_SECTION_START       "(usage\\:)"
#define DOCOPT_RE_USAGE_SECTION_STOP        "((options:)|(\n[\v\t\r\f]*\n)|($))"
#define DOCOPT_RE_USAGE_STATEMENT_START     "((([[:space:]]+)%s)|($))"
#define DOCOPT_RE_OPTION_SECTION_START      "(((options\\:)[[:space:]]*\n *\\-)|($))"
#define DOCOPT_RE_OPTION_STATEMENT_START    "(((\n[ ]*)\\-)|($))"
#define DOCOPT_RE_NAKED_SHORTS              "((\\-([" DOCOPT_RE_ALL_SHORTS_CHARS "]+))([ ]" DOCOPT_RE_UPPER_ARGUMENT ")?)"
#define DOCOPT_RE_ARGED_SHORTS              "((\\-([%s]*[%s]))([ ]?" DOCOPT_RE_UPPER_ARGUMENT ")?)"
#define DOCOPT_RE_OPTREF                    "\\[options\\]"

#define DOCOPT_NUM_PMATCHES                 7

#define DOCOPT_REPEAT                       "..."

struct docopt_entry
{
    struct docopt_str   name;
    char                short_name[3];
    struct docopt_str   command;
    struct docopt_str   defvalue;
};

struct docopt_arg
{
    struct docopt_entry*    owner;
    struct docopt_str       value;
};

struct docopt_entries
{
    int                     error;

    struct docopt_str       docstr;
    unsigned int            flags;

    int                     nentries;
    int                     curentry;
    struct docopt_entry*    entries;

    int                     nargs;
    struct docopt_arg*      args;
};

struct docopt_term
{
    int             entry;
    unsigned int    type        : 5;
    unsigned int    optional    : 1;
    unsigned int    repeat      : 1;
    unsigned int    length      : 20;
};

struct docopt_parse_state
{
    FILE*                   log;
    struct docopt_entries*  entries;
    struct docopt_entries*  args;

    int                     nterms;
    int                     curterm;
    struct docopt_term*     terms;

    struct docopt_str       usage_section;
    struct docopt_str       option_section;
    struct docopt_str       pname;
    char                    arged_shorts[128];
    char                    unarged_shorts[128];
    int                     usage_term;
    int                     args_term;
    int                     optional;
    int                     positional;

    int                     curarg;
    int                     argc;
    int                     nargterms;
    char**                  argv;

    int                     indent;
    int                     scale;
    regmatch_t              docopt_pmatches[DOCOPT_NUM_PMATCHES];
    regex_t                 re_pname;
    regex_t                 re_upper_argument;
    regex_t                 re_argument;
    regex_t                 re_default;
    regex_t                 re_optshort;
    regex_t                 re_long;
    regex_t                 re_repeat;
    regex_t                 re_reserved;
    regex_t                 re_usage_section_start;
    regex_t                 re_usage_section_stop;
    regex_t                 re_usage_statement_start;
    regex_t                 re_option_section_start;
    regex_t                 re_option_statement_start;
    regex_t                 re_naked_shorts;
    regex_t                 re_arged_shorts;
    regex_t                 re_optref;
    regex_t                 re_command;
};

static unsigned docopt_str_len(struct docopt_str* str) { return (unsigned)(str->lst - str->fst); }

static void docopt_set_arg(struct docopt_arg* arg, struct docopt_entry* entry, struct docopt_str* value)
{
    arg->owner  = entry;
    arg->value  = *value;
}

static int docopt_entries_get_entry(struct docopt_entries* dE, struct docopt_str* str)
{
    for (int i = 0; i < dE->curentry; ++i)
    {
        if (((docopt_str_len(str) > 0) &&
                docopt_str_len(str) == docopt_str_len(&dE->entries[i].name)) &&
                !strncmp(str->fst, dE->entries[i].name.fst, docopt_str_len(str)))
            return i;
        if ((docopt_str_len(str) == 2) && (strncmp(str->fst, &dE->entries[i].short_name[0], 2) == 0))
            return i;
    }
    return -1;
}

static struct docopt_entry* docopt_fetch_entry(struct docopt_entries* dE, int hentry)
{
    assert(hentry >= 0);
    return &dE->entries[hentry];
}

static int docopt_entries_insert_entry(struct docopt_entries* dE, struct docopt_entry* entry)
{
    int lentry                  = docopt_entries_get_entry(dE, &entry->name);
    struct docopt_str sname     = { &entry->short_name[0], &entry->short_name[0] + strlen(&entry->short_name[0]) };
    int sentry                  = docopt_entries_get_entry(dE, &sname);
    int hentry                  = -1;
    assert((docopt_str_len(&entry->name) || entry->short_name[0]) && "Invalid ENTRY.");
    if ((lentry >= 0) || (sentry >= 0))
    {
        hentry  = (lentry >= 0) ? lentry : sentry;
        if (!dE->entries[hentry].name.fst) dE->entries[hentry].name = entry->name;
        if (!dE->entries[hentry].command.fst) dE->entries[hentry].command = entry->command;
        if (!dE->entries[hentry].short_name[0]) strcpy(dE->entries[hentry].short_name, entry->short_name);
        if (!dE->entries[hentry].defvalue.fst) dE->entries[hentry].defvalue = entry->defvalue;
    }
    else
    {
        if ((dE->curentry+1) >= dE->nentries)
        {
            dE->nentries    *= 2;
            dE->entries     = (struct docopt_entry*)realloc(dE->entries, sizeof(struct docopt_entry) * dE->nentries);
        }
        hentry              = dE->curentry;
        dE->entries[hentry] = *entry;
        ++dE->curentry;
    }
    return hentry;
}

static struct docopt_term* docopt_fetch_term(struct docopt_parse_state* dS, int hterm)
{
    return &dS->terms[hterm];
}

static int docopt_new_term(struct docopt_parse_state* dS, enum DOCOPT_TYPE type)
{
    int hterm                   = dS->curterm;
    if ((dS->curterm+1) >= dS->nterms)
    {
        dS->nterms      *= 2;
        dS->terms       = realloc(dS->terms, sizeof(struct docopt_term) * dS->nterms);
    }
    memset(docopt_fetch_term(dS, hterm), 0x0, sizeof(struct docopt_term));
    docopt_fetch_term(dS, hterm)->type = type;
    ++dS->curterm;
    return hterm;
}

static void docopt_parse_state_free(struct docopt_parse_state* dS)
{
    if (!dS) return;
    if (dS->terms) free(dS->terms);
    dS->entries     = 0;
    regfree(&dS->re_pname), regfree(&dS->re_upper_argument), regfree(&dS->re_argument),
    regfree(&dS->re_default), regfree(&dS->re_optshort), regfree(&dS->re_long),
    regfree(&dS->re_repeat), regfree(&dS->re_reserved), regfree(&dS->re_usage_section_start),
    regfree(&dS->re_option_section_start), regfree(&dS->re_option_statement_start),
    regfree(&dS->re_naked_shorts), regfree(&dS->re_arged_shorts), regfree(&dS->re_usage_statement_start),
    regfree(&dS->re_optref); regfree(&dS->re_usage_section_start);
}

static int docopt_parse_state_init(struct docopt_parse_state* dS, const char* doc, int argc, char **argv, unsigned int flags)
{
#if defined(DOCOPT_DEBUG)
    dS->log                         = stdout;
#else
    dS->log                         = fopen(DOCOPT_NULL_FILE, "w");
#endif
    if (!(dS->entries = (struct docopt_entries*)calloc(sizeof(struct docopt_entries), 1))) goto docoptFail;
    dS->argc                        = argc;
    dS->argv                        = argv;
    dS->curarg                      = 0;
    dS->entries->docstr.fst         = doc;
    dS->entries->docstr.lst         = doc + strlen(doc);
    dS->entries->flags              = flags;
    dS->entries->nentries           = 2;
    dS->entries->curentry           = 1;
    if (!(dS->entries->entries = (struct docopt_entry*)calloc(sizeof(struct docopt_entry), dS->entries->nentries))) goto docoptFail;
    dS->nterms                      = 1;
    dS->curterm                     = 0;
    if (!(dS->terms = (struct docopt_term*)calloc(sizeof(struct docopt_term), dS->nterms))) goto docoptFail;

    if (regcomp(&dS->re_pname, "^" DOCOPT_RE_PNAME, REG_EXTENDED)) goto docoptFail;
    if (regcomp(&dS->re_upper_argument, "^" DOCOPT_RE_UPPER_ARGUMENT, REG_EXTENDED)) goto docoptFail;
    if (regcomp(&dS->re_argument, "^" DOCOPT_RE_ARGUMENT, REG_EXTENDED)) goto docoptFail;
    if (regcomp(&dS->re_default, DOCOPT_RE_DEFAULT, REG_EXTENDED)) goto docoptFail;
    if (regcomp(&dS->re_optshort, "^" DOCOPT_RE_OPTSHORT, REG_EXTENDED)) goto docoptFail;
    if (regcomp(&dS->re_long, "^" DOCOPT_RE_LONG, REG_EXTENDED)) goto docoptFail;
    if (regcomp(&dS->re_repeat, "^" DOCOPT_RE_REPEAT, REG_EXTENDED)) goto docoptFail;
    if (regcomp(&dS->re_reserved, "^" DOCOPT_RE_RESERVED, REG_EXTENDED)) goto docoptFail;
    if (regcomp(&dS->re_usage_section_start, DOCOPT_RE_USAGE_SECTION_START, REG_EXTENDED | REG_ICASE)) goto docoptFail;
    if (regcomp(&dS->re_usage_section_stop, DOCOPT_RE_USAGE_SECTION_STOP, REG_EXTENDED | REG_ICASE)) goto docoptFail;
    if (regcomp(&dS->re_option_section_start, DOCOPT_RE_OPTION_SECTION_START, REG_EXTENDED | REG_ICASE)) goto docoptFail;
    if (regcomp(&dS->re_option_statement_start, DOCOPT_RE_OPTION_STATEMENT_START, REG_EXTENDED)) goto docoptFail;
    if (regcomp(&dS->re_naked_shorts, "^" DOCOPT_RE_NAKED_SHORTS, REG_EXTENDED)) goto docoptFail;
    if (regcomp(&dS->re_optref, "^" DOCOPT_RE_OPTREF, REG_EXTENDED)) goto docoptFail;

    return 0;
docoptFail:
    return 1;
}

/*====[ After this point it is illegal to declare docopt_term*, docopt_entry*, or docopt_arg*.]====*/

#define DO_TM(TERM) docopt_fetch_term(dS, (TERM))

static void docopt_eat_ws(struct docopt_str* str)
{
    while ((str->fst < str->lst) && isspace(str->fst[0])) ++str->fst;
}

static void docopt_strip(struct docopt_str* str, const char* what)
{
    while ((str->fst < str->lst) && strchr(what, str->fst[0])) ++str->fst;
    while ((str->lst >= str->fst) && strchr(what, str->lst[-1])) --str->lst;
}

static int docopt_get_section(struct docopt_parse_state* dS, regex_t* start, regex_t* stop, const char* fst, struct docopt_str* sec, int pidx, int qidx)
{
    if (dS->entries->error) return 1;
    if (regexec(start, fst, DOCOPT_NUM_PMATCHES, &dS->docopt_pmatches[0], 0))
        return 1;
    if (dS->docopt_pmatches[pidx].rm_so < 0) return 1;
    sec->fst    = dS->docopt_pmatches[pidx].rm_eo + fst;
    if (regexec(stop, sec->fst, DOCOPT_NUM_PMATCHES, &dS->docopt_pmatches[0], 0)) return 1;
    if (dS->docopt_pmatches[qidx].rm_so < 0) return 1;
    sec->lst    = dS->docopt_pmatches[qidx].rm_so + sec->fst;
    return 0;
}

static int docopt_parse_options(struct docopt_parse_state* dS)
{
    if (dS->entries->error) return 1;
    struct docopt_str optstmt   = { 0 };
    struct docopt_entry option  = { };
    int which_first             = 0;
    char unarged_shorts[]       = DOCOPT_RE_ALL_SHORTS_CHARS;
    char re_arged_shorts[256]   = { 0 };
    char short_name[3]          = "- ";
    struct docopt_str sname     = { &short_name[0], &short_name[2] };
    int options_x               = 0;
    int options_s               = 0;
    int sterm                   = 0;
    int lterm                   = 0;

    if (docopt_get_section(dS, &dS->re_option_section_start, &dS->re_option_section_start, dS->usage_section.lst, &dS->option_section, 3, 0)) return 0;
    dS->option_section.lst  = dS->entries->docstr.lst;
    optstmt.fst                 = dS->option_section.fst;

    options_x                   = docopt_new_term(dS, DOCOPT_TYPE_OPTION_SECTION);
    DO_TM(options_x)->repeat    = 1;
    DO_TM(options_x)->optional  = 1;
    while (!docopt_get_section(dS, &dS->re_option_statement_start, &dS->re_option_statement_start, optstmt.fst, &optstmt, 3, 0))
    {
        fprintf(dS->log, "%s\n", "Parsing Option Statement.");
        sterm           = -1;
        lterm           = -1;
        which_first     = 0;
        memset(&option, 0x0, sizeof(option));
        if (optstmt.fst >= dS->option_section.lst) break;
        docopt_eat_ws(&optstmt);
        options_s   = docopt_new_term(dS, DOCOPT_TYPE_OPTION_STATEMENT);
        for (int X = 0; X < 2; ++X)
        {
            if ((which_first != 1) && regexec(&dS->re_optshort, optstmt.fst, DOCOPT_NUM_PMATCHES, &dS->docopt_pmatches[0], 0) == 0)
            {
                sterm       = docopt_new_term(dS, DOCOPT_TYPE_SHORT);
                DO_TM(sterm)->length = 1;
                which_first = 1;
                sprintf(option.short_name, "-%c", optstmt.fst[1]);
                if (dS->docopt_pmatches[4].rm_so >= 0)
                {
                    option.command.fst  = optstmt.fst + dS->docopt_pmatches[4].rm_so;
                    option.command.lst  = optstmt.fst + dS->docopt_pmatches[4].rm_eo;
                }
            }
            else if ((which_first != 2) && regexec(&dS->re_long, optstmt.fst, DOCOPT_NUM_PMATCHES, &dS->docopt_pmatches[0], 0) == 0)
            {
                which_first = 2;
                lterm           = docopt_new_term(dS, DOCOPT_TYPE_LONG);
                DO_TM(lterm)->length = 1;
                option.name.fst = optstmt.fst;
                option.name.lst = optstmt.fst + dS->docopt_pmatches[2].rm_eo;
                if (dS->docopt_pmatches[4].rm_so >= 0)
                {
                    option.command.fst  = optstmt.fst + dS->docopt_pmatches[4].rm_so;
                    option.command.lst  = optstmt.fst + dS->docopt_pmatches[4].rm_eo;
                }
            }
            if (X == 0) optstmt.fst += dS->docopt_pmatches[0].rm_eo + 1; docopt_eat_ws(&optstmt);
        }
        if (which_first == 0)
        {
            dS->entries->error  = DOCOPT_BAD_DEFAULTS;
            return 1;
        }
        if (regexec(&dS->re_default, optstmt.fst, DOCOPT_NUM_PMATCHES, &dS->docopt_pmatches[0], 0)) goto docoptAddOption;
        if ((optstmt.fst + dS->docopt_pmatches[0].rm_eo) > optstmt.lst) goto docoptAddOption;
        option.defvalue.fst     = optstmt.fst + dS->docopt_pmatches[2].rm_so;
        option.defvalue.lst     = optstmt.fst + dS->docopt_pmatches[2].rm_eo;
docoptAddOption:
        if (lterm >= 0) DO_TM(lterm)->entry = docopt_entries_insert_entry(dS->entries, &option);
        if (sterm >= 0) DO_TM(sterm)->entry = docopt_entries_insert_entry(dS->entries, &option);
        DO_TM(options_s)->length = dS->curterm - options_s;
        fprintf(dS->log, "%s\n", "... Done Parsing Option Statement.");
    }
    DO_TM(options_x)->length = dS->curterm - options_x;
    for (int i = 0, idx = 0, jdx = 0, entry; i < strlen(unarged_shorts); ++i)
    {
        short_name[1]   = unarged_shorts[i];
        if (((entry = docopt_entries_get_entry(dS->entries, &sname)) < 0) ||
            !docopt_str_len(&docopt_fetch_entry(dS->entries, entry)->command))
            dS->unarged_shorts[idx++]   = unarged_shorts[i];
        else
            dS->arged_shorts[jdx++]     = short_name[1];
    }
    if (strlen(dS->arged_shorts))
    {
        fprintf(dS->log, "Setting up arged shorts: %s\n", dS->arged_shorts);
        sprintf(&re_arged_shorts[0], "^" DOCOPT_RE_ARGED_SHORTS, dS->unarged_shorts, dS->arged_shorts);
        if (regcomp(&dS->re_arged_shorts, re_arged_shorts, REG_EXTENDED))
        {
            dS->entries->error  = DOCOPT_BAD_ARGED_SHORTS;
            return 1;
        }
    }
    return 0;
}

static int docopt_parse_seq(struct docopt_parse_state* dS, struct docopt_str* usestmt);
static int docopt_parse_atom(struct docopt_parse_state* dS, struct docopt_str* usestmt);

static int docopt_parse_expr(struct docopt_parse_state* dS, struct docopt_str* usestmt)
{
    if (dS->entries->error) return 0;
    struct docopt_str olduse    = *usestmt;
    int hterm                   = docopt_new_term(dS, DOCOPT_TYPE_EXPR);
    DO_TM(hterm)->optional = dS->optional;
    docopt_eat_ws(usestmt);
    while ((usestmt->fst < usestmt->lst) && docopt_parse_seq(dS, usestmt))
    {
        docopt_eat_ws(usestmt);
        if (usestmt->fst[0] != '|') break;
        ++usestmt->fst;
        docopt_eat_ws(usestmt);
    }
    if ((hterm + 1) == dS->curterm)
    {
        *usestmt                    = olduse;
        dS->curterm                 = hterm;
        return 0;
    }
    DO_TM(hterm)->length = dS->curterm - hterm;
    return 1;
}

int docopt_parse_seq(struct docopt_parse_state* dS, struct docopt_str* usestmt)
{
    if (dS->entries->error) return 0;
    struct docopt_str olduse    = *usestmt;
    int hterm                   = docopt_new_term(dS, DOCOPT_TYPE_SEQ);
    int atom                    = -1;
    DO_TM(hterm)->optional = dS->optional;
    docopt_eat_ws(usestmt);
    atom                        = dS->curterm;
    while ((usestmt->fst < usestmt->lst) && docopt_parse_atom(dS, usestmt))
    {
        docopt_eat_ws(usestmt);
        if (strncmp(DOCOPT_REPEAT, usestmt->fst, strlen(DOCOPT_REPEAT)) == 0)
        {
            DO_TM(atom)->repeat = 1;
            usestmt->fst        += 3;
        }
        atom                    = dS->curterm;
    }
    if ((hterm + 1) == dS->curterm)
    {
        *usestmt        = olduse;
        dS->curterm     = hterm;
        return 0;
    }
    DO_TM(hterm)->length = dS->curterm - hterm;
    return 1;
}

static void docopt_parse_command(struct docopt_parse_state* dS, struct docopt_str* usestmt)
{
    if (dS->entries->error) return;
    while (
        (usestmt->fst < usestmt->lst) &&
        (strncmp(DOCOPT_REPEAT, usestmt->fst, strlen(DOCOPT_REPEAT)) != 0) &&
        (strchr("()[]|", usestmt->fst[0]) == 0) &&
        !isspace(usestmt->fst[0]) &&
        1)
        ++usestmt->fst;
}

int docopt_parse_atom(struct docopt_parse_state* dS, struct docopt_str* usestmt)
{
    if (dS->entries->error) return 0;
    struct docopt_str olduse    = *usestmt;
    struct docopt_entry entry   = { };
    int hterm                   = -1;
    char seqend                 = 0;

    if (usestmt->fst >= usestmt->lst) goto docoptFail;
    docopt_eat_ws(usestmt);

    if (strncmp(dS->pname.fst, usestmt->fst, docopt_str_len(&dS->pname)) == 0)
    {
        hterm           = docopt_new_term(dS, DOCOPT_TYPE_PNAME);
        usestmt->fst    += docopt_str_len(&dS->pname);
        entry.name      = dS->pname;
        DO_TM(hterm)->entry = docopt_entries_insert_entry(dS->entries, &entry);
    }
    else if (!regexec(&dS->re_optref, usestmt->fst, DOCOPT_NUM_PMATCHES, &dS->docopt_pmatches[0], 0))
    {
        hterm           = docopt_new_term(dS, DOCOPT_TYPE_OPTREF);
        usestmt->fst    += dS->docopt_pmatches[0].rm_eo;
    }
    else if (strchr("[(", usestmt->fst[0]) != 0)
    {
        hterm           = dS->curterm;
        ++usestmt->fst;
        dS->optional    = (olduse.fst[0] == '[');
        seqend          = (olduse.fst[0] == '[') ? ']' : ')';
        hterm           = dS->curarg;
        if (!docopt_parse_expr(dS, usestmt)) goto docoptFail;
        DO_TM(hterm)->optional = dS->optional;
        hterm           = -1;
        docopt_eat_ws(usestmt);
        if (seqend != usestmt->fst[0]) goto docoptFail;
        ++usestmt->fst;
        dS->optional    = 0;
    }
    else if (!regexec(&dS->re_long, usestmt->fst, DOCOPT_NUM_PMATCHES, &dS->docopt_pmatches[0], 0))
    {
        hterm           = docopt_new_term(dS, DOCOPT_TYPE_LONG);
        entry.name.fst  = usestmt->fst;
        entry.name.lst  = usestmt->fst + dS->docopt_pmatches[2].rm_eo;
        if (dS->docopt_pmatches[4].rm_so >= 0)
        {
            entry.command.fst  = usestmt->fst + dS->docopt_pmatches[4].rm_so;
            entry.command.lst  = usestmt->fst + dS->docopt_pmatches[4].rm_eo;
        }
        DO_TM(hterm)->entry = docopt_entries_insert_entry(dS->entries, &entry);
        usestmt->fst    += dS->docopt_pmatches[0].rm_eo;
    }
    else if ((dS->arged_shorts[0] && !regexec(&dS->re_arged_shorts, usestmt->fst, DOCOPT_NUM_PMATCHES, &dS->docopt_pmatches[0], 0)) ||
        !regexec(&dS->re_naked_shorts, usestmt->fst, DOCOPT_NUM_PMATCHES, &dS->docopt_pmatches[0], 0))
    {
        for (int h = dS->docopt_pmatches[3].rm_so, he = dS->docopt_pmatches[3].rm_eo; h < he; ++h)
        {
            sprintf(&entry.short_name[0], "-%c", usestmt->fst[h]);
            hterm           = docopt_new_term(dS, DOCOPT_TYPE_SHORT);
            DO_TM(hterm)->length = 1;
            if (((h+1) == dS->docopt_pmatches[3].rm_eo) && (dS->docopt_pmatches[5].rm_so > 0))
            {
                entry.command.fst   = usestmt->fst + dS->docopt_pmatches[5].rm_so;
                entry.command.lst   = usestmt->fst + dS->docopt_pmatches[5].rm_eo;
            }
            DO_TM(hterm)->optional = dS->optional;
            DO_TM(hterm)->entry = docopt_entries_insert_entry(dS->entries, &entry);
        }
        usestmt->fst    += dS->docopt_pmatches[0].rm_eo;
    }
    else if (!regexec(&dS->re_argument, usestmt->fst, DOCOPT_NUM_PMATCHES, &dS->docopt_pmatches[0], 0))
    {
        hterm           = docopt_new_term(dS, DOCOPT_TYPE_ARGUMENT);
        entry.name.fst  = usestmt->fst;
        entry.name.lst  = usestmt->fst + dS->docopt_pmatches[1].rm_eo;
        DO_TM(hterm)->entry = docopt_entries_insert_entry(dS->entries, &entry);
        usestmt->fst    += dS->docopt_pmatches[0].rm_eo;
    }
    else
    {
        hterm           = docopt_new_term(dS, DOCOPT_TYPE_COMMAND);
        docopt_parse_command(dS, usestmt);
        if (olduse.fst == usestmt->fst) goto docoptFail;
        entry.name.fst  = olduse.fst;
        entry.name.lst  = usestmt->fst;
        DO_TM(hterm)->entry = docopt_entries_insert_entry(dS->entries, &entry);
    }

    if (olduse.fst == usestmt->fst) goto docoptFail;
    if (hterm >= 0)
    {
        DO_TM(hterm)->optional  = dS->optional;
        DO_TM(hterm)->length    = dS->curterm - hterm;
    }
    return hterm;
docoptFail:
    *usestmt        = olduse;
    dS->curterm     = hterm;
    return 0;
}

static int docopt_parse_usage(struct docopt_parse_state* dS)
{
    if (dS->entries->error) return 0;
    char pname[512]             = { 0 };
    char re_pname[512]          = { 0 };
    struct docopt_str   usestmt = { 0, 0 };
    int usage_section_s         = 0;
    for (int i = 0, e = docopt_str_len(&dS->pname); i < e; ++i)
    {
        pname[2*i+0] = '\\';
        pname[2*i+1] = dS->pname.fst[i];
    }
    sprintf(&re_pname[0], DOCOPT_RE_USAGE_STATEMENT_START, pname);
    if (regcomp(&dS->re_usage_statement_start, re_pname, REG_EXTENDED))
    {
        dS->entries->error  = DOCOPT_BAD_USAGE_NAME;
        return 1;
    }

    dS->usage_term  = docopt_new_term(dS, DOCOPT_TYPE_USAGE_SECTION);
    usestmt.fst     = dS->usage_section.fst;
    while (!docopt_get_section(dS, &dS->re_usage_statement_start, &dS->re_usage_statement_start, usestmt.fst, &usestmt, 3, 0))
    {
        if (usestmt.fst >= dS->usage_section.lst) break;
        usestmt.lst     = usestmt.lst > dS->usage_section.lst ? dS->usage_section.lst : usestmt.lst;
        fprintf(dS->log, "USAGE SECTION(%.*s)\n", DOCOPT_PR(&usestmt));
        usage_section_s = docopt_new_term(dS, DOCOPT_TYPE_USAGE_STATEMENT);
        if (!docopt_parse_expr(dS, &usestmt))
        {
            dS->entries->error   = DOCOPT_BAD_USAGE_STATEMENT;
            return 1;
        }
        fprintf(dS->log, "...RETRIEVED(%d)\n", dS->curterm - usage_section_s);
        DO_TM(usage_section_s)->length  = dS->curterm - usage_section_s;
    }
    DO_TM(dS->usage_term)->length       = dS->curterm - dS->usage_term;

    return 0;
}

static int docopt_unify_args(struct docopt_parse_state* dS, int hterm)
{
    if (dS->entries->error) return 0;
    int result                  = 0;
    int curarg                  = dS->curarg;
    struct docopt_term* term    = DO_TM(hterm);
    struct docopt_term* argterm = (dS->curarg < dS->nargterms)
                                ? DO_TM(dS->args_term + dS->curarg)
                                : 0;
    struct docopt_entry* terme  = docopt_fetch_entry(dS->entries, term->entry);
    struct docopt_entry* terma  = argterm
                                ? docopt_fetch_entry(dS->args, argterm->entry)
                                : 0;
    dS->indent                  = 0;
    dS->scale                   = 2;

    fprintf(dS->log, "%*s%s(%.*s%s)[%d]%s%s\n",
        dS->scale*dS->indent, "",
        DOCOPT_TYPE_NAMES[term->type],
        DOCOPT_PR(&terme->name),
        terme->short_name,
        term->length,
        term->optional ? "?" : " ",
        term->repeat ? "..." : "   ");

    if ((term->type != DOCOPT_TYPE_OPTION_SECTION) &&
        !dS->optional && !dS->positional
        && (dS->entries->flags & DOCOPT_NO_OPTIONS_FIRST))
        docopt_unify_args(dS, 0);

    switch (term->type)
    {
    case DOCOPT_TYPE_USAGE_SECTION      :
        ++dS->indent;
        dS->curarg  = 0;
        curarg      = 0;
        for (int i = 1; i < term->length; i += DO_TM(hterm+i)->length)
            if (docopt_unify_args(dS, hterm+i) && (dS->nargterms == dS->curarg))
                break;
            else
            {
                dS->positional  = 0;
                for (int i = curarg; i < dS->nargterms; ++i)
                    dS->entries->args[i].owner  = 0;
                dS->curarg      = curarg;
            }
        result  = dS->nargterms == dS->curarg;
        --dS->indent;
        break;
    case DOCOPT_TYPE_USAGE_STATEMENT    :
        ++dS->indent;
        for (int i = 0; i < dS->nargterms; ++i)
            dS->entries->args[i].owner  = 0;
        dS->positional  = 0;
        dS->optional    = 0;
        result          = docopt_unify_args(dS, hterm+1);
        dS->optional    = 1;
        --dS->indent;
        break;
    case DOCOPT_TYPE_EXPR               :
        ++dS->indent;
        for (int i = 1; i < term->length; i += DO_TM(hterm+i)->length)
            if ((result = docopt_unify_args(dS, hterm+i)))
                break;
        result |= term->optional;
        --dS->indent;
        break;
    case DOCOPT_TYPE_SEQ                :
        ++dS->indent;
        if (!term->optional)
        {
            for (int i = 1; i < term->length; i += DO_TM(hterm+i)->length)
                if (!(result = docopt_unify_args(dS, hterm+i)))
                    break;
        }
        else
        {
            result = 0;
            for (int i = 1; i < term->length; i += DO_TM(hterm+i)->length)
                if (docopt_unify_args(dS, hterm+i))
                {
                    result = 1;
                }
        }
        --dS->indent;
        break;
    case DOCOPT_TYPE_PNAME              :
        result = 1;
        break;
    case DOCOPT_TYPE_OPTREF             :
        ++dS->indent;
        docopt_unify_args(dS, 0);
        result = 1;
        --dS->indent;
        break;
    case DOCOPT_TYPE_OPTION_SECTION     :
        if (dS->curarg >= dS->nargterms) break;
        ++dS->indent;
        dS->optional    = 1;
        for (int i = 1; i < term->length; i += DO_TM(hterm+i)->length)
            if ((result = docopt_unify_args(dS, hterm+i)))
                break;
        dS->optional    = 0;
        --dS->indent;
        break;
    case DOCOPT_TYPE_OPTION_STATEMENT   :
        if (dS->curarg >= dS->nargterms) break;
        ++dS->indent;
        for (int i = 1; i < term->length; i += DO_TM(hterm+i)->length)
            if (docopt_unify_args(dS, hterm+i))
                result  = 1;
        --dS->indent;
        break;
docoptUnifyShort:
    case DOCOPT_TYPE_SHORT              :
        if (dS->curarg >= dS->nargterms) break;
        if (argterm->type != DOCOPT_TYPE_SHORT) break;
        if (strncmp(terme->short_name, terma->short_name, 2))
        {
            if ((term->type == DOCOPT_TYPE_SHORT) && terme->name.fst)
                goto docoptUnifyLong;
            break;
        }
        fprintf(dS->log, "%*s  ...%s %s (rpt:%d)\n", dS->scale*dS->indent, "", terme->short_name, terma->short_name, term->repeat);
        if (terme->command.fst)
        {
            if (!terma->command.fst)
            {
                if (dS->terms[dS->args_term+dS->curarg+1].type != DOCOPT_TYPE_ARGUMENT)
                    break;
                ++dS->curarg;
                terma   = docopt_fetch_entry(dS->args, DO_TM(dS->args_term + dS->curarg)->entry);
                docopt_set_arg(dS->entries->args+dS->curarg, terme, &terma->name);
            }
            else docopt_set_arg(dS->entries->args+dS->curarg, terme, &terma->command);
        }
        else docopt_set_arg(dS->entries->args+dS->curarg, terme, &terma->name);
        fprintf(dS->log, "%*s   [0x%010llX]\n", dS->scale*dS->indent, "", (uint64_t)dS->entries->args[dS->curarg].owner);
        ++dS->curarg;
        result = 1;
        break;
docoptUnifyLong:
    case DOCOPT_TYPE_LONG               :
        if (dS->curarg >= dS->nargterms) break;
        if (argterm->type != DOCOPT_TYPE_LONG) break;
        if (strncmp(terme->name.fst, terma->name.fst, docopt_str_len(&terme->name)))
        {
            if ((term->type == DOCOPT_TYPE_LONG) && terme->short_name[0])
                goto docoptUnifyShort;
            break;
        }
        if (terme->command.fst)
        {
            if (!terma->command.fst)
            {
                if (dS->terms[dS->args_term+dS->curarg+1].type != DOCOPT_TYPE_ARGUMENT)
                    break;
                ++dS->curarg;
                terma   = docopt_fetch_entry(dS->args, DO_TM(dS->args_term + dS->curarg)->entry);
                docopt_set_arg(dS->entries->args+dS->curarg, terme, &terma->name);
            }
            else docopt_set_arg(dS->entries->args+dS->curarg, terme, &terma->command);
        }
        else docopt_set_arg(dS->entries->args+dS->curarg, terme, &terma->name);
        ++dS->curarg;
        result = 1;
        break;
    case DOCOPT_TYPE_ARGUMENT           :
        if (dS->curarg >= dS->nargterms) break;
        if (argterm->type != DOCOPT_TYPE_ARGUMENT) break;
        docopt_set_arg(dS->entries->args+dS->curarg, terme, &terma->name);
        ++dS->curarg;
        result = 1;
        break;
    case DOCOPT_TYPE_COMMAND            :
        if (dS->curarg >= dS->nargterms) break;
        if (argterm->type != DOCOPT_TYPE_ARGUMENT) break;
        if (strncmp(terme->name.fst, terma->name.fst, docopt_str_len(&terme->name))) break;
        if (!strcmp(terma->name.fst, "--")) dS->positional = 1;
        docopt_set_arg(dS->entries->args+dS->curarg, terme, &terma->name);
        ++dS->curarg;
        result = 1;
        break;
    }

    if (!result)
    {
        dS->positional  = 0;
        for (int i = curarg; i < dS->nargterms; ++i)
            dS->entries->args[i].owner  = 0;
        dS->curarg      = curarg;
    }

    if ((curarg < dS->entries->nargs) && (curarg < dS->curarg) && term->repeat && result)
    {
        fprintf(dS->log, "%*s%s(%d < %d)\n", dS->scale*dS->indent, "", "Repeating", curarg, dS->curarg);
        docopt_unify_args(dS, hterm);
    }

    fprintf(dS->log, "%*s%s\n",
        dS->scale*dS->indent, "", result ? "PASSED" : "FAILED");

    return result;
}

static int docopt_parse_args(struct docopt_parse_state* dS)
{
    if (dS->entries->error) return 0;
    // options* commands* -- arguments*
    struct docopt_entries argE  = *dS->entries;
    struct docopt_entries *docE = dS->entries;
    int result                  = 0;
    int hterm                   = 0;
    struct docopt_str arg       = { 0, 0 };
    struct docopt_entry entry   = { { 0 } };
    struct docopt_entry* terme  = 0;
    argE.nentries               = dS->argc+1;
    argE.curentry               = 0;
    if (!(argE.entries = (struct docopt_entry*)calloc(sizeof(struct docopt_entry), dS->argc+1)))
        goto docoptDone;
    dS->entries                 = &argE;

    dS->args_term               = dS->curterm;
    for (int i = 0; i < dS->argc; ++i)
    {
        arg.fst         = dS->argv[i];
        arg.lst         = dS->argv[i] + strlen(dS->argv[i]);
        if (!strcmp(dS->argv[i], "--") || !strcmp(dS->argv[i], "-") || (dS->argv[i][0] != '-') || dS->positional)
        {
            if (!strcmp(dS->argv[i], "--")) dS->positional = 1;
            fprintf(dS->log, "ARGUMENT (%s)\n", dS->argv[i]);
            hterm       = docopt_new_term(dS, DOCOPT_TYPE_ARGUMENT);
            DO_TM(hterm)->length = 1;
            entry.name.fst  = dS->argv[i];
            entry.name.lst  = dS->argv[i] + strlen(dS->argv[i]);
            DO_TM(hterm)->entry = docopt_entries_insert_entry(dS->entries, &entry);
        }
        else
        {
            fprintf(dS->log, "LONG/SHORT (%s)\n", dS->argv[i]);
            hterm           = docopt_parse_atom(dS, &arg);
            if (DO_TM(hterm)->type == DOCOPT_TYPE_LONG)
            {
                terme       = docopt_fetch_entry(dS->entries, DO_TM(hterm)->entry);
                if (docopt_str_len(&terme->name) < strlen(dS->argv[i]))
                {
                    terme->command.fst  = dS->argv[i] + 1 + docopt_str_len(&terme->name);
                    terme->command.lst  = dS->argv[i] + strlen(dS->argv[i]);
                }
            }
            if ((DO_TM(hterm)->type == DOCOPT_TYPE_SHORT) && (strlen(dS->argv[i]) > 2))
            {
                terme               = docopt_fetch_entry(dS->entries, DO_TM(hterm)->entry);
                terme->command.fst  = dS->argv[i] + 2;
                terme->command.lst  = dS->argv[i] + strlen(dS->argv[i]);
            }
        }
    }

    dS->optional    = 1;
    dS->positional  = 0;
    dS->entries     = docE;
    dS->args        = &argE;

    dS->nargterms       = dS->curterm - dS->args_term;
    dS->entries->nargs  = dS->nargterms;
    fprintf(dS->log, "Tokenized Args Length (%d)\n", dS->entries->nargs);
    if (!(dS->entries->args = (struct docopt_arg*)calloc(sizeof(struct docopt_arg), dS->entries->nargs)))
    {
        dS->entries->error = DOCOPT_BAD_ARG_ALLOC;
        return 1;
    }

    result          = docopt_unify_args(dS, dS->usage_term);

docoptDone:
    dS->entries                 = docE;
    if (argE.entries) free(argE.entries);
    return !result;
}

void docopt_print_help(struct docopt_parse_state* dS, int moreInfo)
{
    dS->entries->error  = DOCOPT_PRINTED_HELP;
    if (dS->entries->flags & DOCOPT_NO_HELP) return;
    if (moreInfo)
    {
        docopt_strip(&dS->entries->docstr, "\n");
        fprintf(stderr, "%.*s\n", DOCOPT_PR((&dS->entries->docstr)));
    }
    else
    {
        fprintf(stderr, "%s", "Usage:\n");
        docopt_strip(&dS->usage_section, "\n");
        fprintf(stderr, "%.*s\n", DOCOPT_PR((&dS->usage_section)));
    }
}

docopt_t docopt(const char* doc, int argc, char** argv, unsigned int flags, unsigned int version)
{
    struct docopt_parse_state   dS  = { 0 };
    struct docopt_entries*      dE  = 0;
    uint32_t VERSION                = DOCOPT_VERSION;
    if (((version & 0x00FF0000) != (VERSION & 0x00FF0000)) || (version < VERSION))
    {
        dS.entries->error           = DOCOPT_BAD_VERSION;
        goto docoptFail;
    }
    if (docopt_parse_state_init(&dS, doc, argc, argv, flags)) goto docoptFail;
    if (dS.entries->error) goto docoptFail;
    fprintf(dS.log, "%s", "Getting Usage Section.\n");
    if (docopt_get_section(&dS, &dS.re_usage_section_start, &dS.re_usage_section_stop, dS.entries->docstr.fst, &dS.usage_section, 0, 0)) goto docoptFail;
    if (dS.entries->error) goto docoptFail;
    fprintf(dS.log, "%s", "Getting Usage PNAME... ");
    if (regexec(&dS.re_pname, dS.usage_section.fst, DOCOPT_NUM_PMATCHES, &dS.docopt_pmatches[0], 0)) return 0;
    if (dS.entries->error) goto docoptFail;
    fprintf(dS.log, "" PRdocS "\n", DOCOPT_PR(&dS.pname));
    dS.pname.fst    = dS.docopt_pmatches[2].rm_so + dS.usage_section.fst;
    dS.pname.lst    = dS.docopt_pmatches[2].rm_eo + dS.usage_section.fst;
    fprintf(dS.log, "%s", "Getting Parse Options.\n");
    if (docopt_parse_options(&dS)) goto docoptFail;
    if (dS.entries->error) goto docoptFail;
    fprintf(dS.log, "%s", "Getting Parse Usage.\n");
    if (docopt_parse_usage(&dS)) goto docoptFail;
    if (dS.entries->error) goto docoptFail;
    dE      = dS.entries;
    fprintf(dS.log, "Getting Parse Args (%d).\n", argc);
    if (argc && docopt_parse_args(&dS))
    {
        goto docoptFail;
    }

    fprintf(dS.log, "%s", "Success!\n");

docoptFail:
    for (int i = 0; i < argc; ++i)
        if (!strncmp(argv[i], "--help", 6) || !strncmp(argv[i], "-h", 2))
        {
            docopt_print_help(&dS, 1);
            return dE;
        }
    if ((argc == 0) || !dE) docopt_print_help(&dS, 0);
    docopt_parse_state_free(&dS);
    return dE;
}

int docopt_error(docopt_t doc)
{
    return ((struct docopt_entries*)doc)->error;
}

docopt_str_t* docopt_get(docopt_t doc, const char* cmd, int idx)
{
    struct docopt_entries* dE   = (struct docopt_entries*)doc;
    assert(!dE->error || (dE->error == DOCOPT_PRINTED_HELP));
    struct docopt_str str       = { cmd, cmd + strlen(cmd) };
    struct docopt_entry* owner  = 0;
    struct docopt_str* result   = 0;
    int entry                   = 0;
    if ((entry = docopt_entries_get_entry(dE, &str)) < 0)
        return 0;
    owner                       = docopt_fetch_entry(dE, entry);
    for (int i = 0; (i < dE->nargs) && (idx >= 0); ++i)
        if (dE->args[i].owner == owner)
        {
            if (dE->args[i].value.fst)
                result  = &dE->args[i].value;
            else
                result  = &owner->defvalue;
            --idx;
        }
    if ((idx >= 0) && owner->defvalue.fst)
    {
        return &owner->defvalue;
    }
    return (idx < 0) ? result : 0;
}

void docopt_free(docopt_t doc)
{
    struct docopt_entries* dE   = (struct docopt_entries*)doc;
    if (dE)
    {
        if (dE->entries) free(dE->entries);
        if (dE->args) free(dE->args);
        free(dE);
    }
}

#undef DO_TM

#endif/*DOCOPT_C*/
