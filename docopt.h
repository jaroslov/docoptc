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

#define DOCOPT_OK                   0
#define DOCOPT_BAD_DEFAULTS         1
#define DOCOPT_BAD_USAGE_NAME       2
#define DOCOPT_BAD_USAGE_STATEMENT  3
#define DOCOPT_BAD_ARGED_SHORTS     4
#define DOCOPT_PRINTED_HELP         5

#define DOCOPT_NO_HELP              0x1
#define DOCOPT_HELP                 0x0
#define DOCOPT_SIMPLE_BACKEND       0x2

typedef void* docopt_t;
typedef struct docopt_str { const char* fst; const char* lst; } docopt_str_t;

docopt_t docopt(const char* doc, int argc, char** argv, char* version, unsigned int flags);
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

#define DOCOPT_TYPE_TBL(X) X(BAD) X(LONG) X(SHORTS) X(SHORT) X(OPTREF) X(PNAME) X(COMMAND) X(ARGUMENT) X(SEQ) X(EXPR) X(MAX)
enum DOCOPT_TYPE
{
#define DOCOPT_TYPE_ENUM(N) DOCOPT_TYPE_##N,
DOCOPT_TYPE_TBL(DOCOPT_TYPE_ENUM)
};
const char* DOCOPT_TYPE_NAMES[] =
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
#define DOCOPT_RE_USAGE_STATEMENT_START     "((([[:space:]]+)%s)|($))"
#define DOCOPT_RE_OPTION_SECTION_START      "(((options\\:)[[:space:]]*\n *\\-)|($))"
#define DOCOPT_RE_OPTION_STATEMENT_START    "(((\n[ ]*)\\-)|($))"
#define DOCOPT_RE_NAKED_SHORTS              "((\\-([" DOCOPT_RE_ALL_SHORTS_CHARS "]+))([ ]" DOCOPT_RE_UPPER_ARGUMENT ")?)"
#define DOCOPT_RE_ARGED_SHORTS              "((\\-([%s]*[%s]))([ ]?" DOCOPT_RE_UPPER_ARGUMENT ")?)"
#define DOCOPT_RE_OPTREF                    "\\[options\\]"

#define DOCOPT_NUM_PMATCHES                 7

#define DOCOPT_REPEAT                       "..."

static regmatch_t docopt_pmatches[DOCOPT_NUM_PMATCHES] = { };

#define DOCOPT_PR(str) docopt_str_len(str), str->fst

struct docopt_arg
{
    struct docopt_str   value;
    struct docopt_arg*  next;
};

struct docopt_entry
{
    struct docopt_str   name;
    char                short_name[3];
    struct docopt_str   command;
    struct docopt_str   defvalue;
    struct docopt_arg*  arg;
};

struct docopt_entries
{
    int                     error;

    struct docopt_str       docstr;
    char*                   version;
    unsigned int            flags;

    int                     nentries;
    int                     curentry;
    struct docopt_entry*    entries;

    struct docopt_arg*      args;
};

struct docopt_term
{
    int             entry;
    unsigned int    type        : 5;
    unsigned int    optional    : 1;
    unsigned int    repeat      : 1;
    unsigned int    eat_all     : 1;
    unsigned int    length      : 20;
};

struct docopt_parse_state
{
    FILE*                   log;
    struct docopt_entries*  entries;

    int                     nterms;
    int                     curterm;
    struct docopt_term*     terms;

    struct docopt_str       usage_section;
    struct docopt_str       option_section;
    struct docopt_str       pname;
    char                    arged_shorts[128];
    char                    unarged_shorts[128];
    int                     usage_term;
    int                     positional;

    int                     curarg;
    int                     curargoff;
    int                     argc;
    char**                  argv;

    regex_t                 re_pname;
    regex_t                 re_upper_argument;
    regex_t                 re_argument;
    regex_t                 re_default;
    regex_t                 re_optshort;
    regex_t                 re_long;
    regex_t                 re_repeat;
    regex_t                 re_reserved;
    regex_t                 re_usage_section_start;
    regex_t                 re_usage_statement_start;
    regex_t                 re_option_section_start;
    regex_t                 re_option_statement_start;
    regex_t                 re_naked_shorts;
    regex_t                 re_arged_shorts;
    regex_t                 re_optref;
    regex_t                 re_command;
};

static int docopt_str_len(struct docopt_str* str) { return str->lst - str->fst; }

static int docopt_entries_get_entry(struct docopt_entries* dE, struct docopt_str* str)
{
    for (int i = 0; i < dE->curentry; ++i)
    {
        if (((docopt_str_len(str) > 0) &&
                docopt_str_len(str) == docopt_str_len(&dE->entries[i].name)) &&
                (strncmp(str->fst, dE->entries[i].name.fst, docopt_str_len(str)) == 0))
            return i;
        if ((docopt_str_len(str) == 2) && (strncmp(str->fst, &dE->entries[i].short_name[0], 2) == 0))
            return i;
    }
    return -1;
}

static struct docopt_entry* docopt_entries_deref(struct docopt_entries* dE, int hentry)
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

static void docopt_add_arg_list(struct docopt_parse_state* dS, struct docopt_entries* dE, struct docopt_entry* entry, int argoff, int off, int atend)
{
    struct docopt_arg*  arg = 0;
    if ((entry->arg == 0) || !atend)
    {
        entry->arg  = &dE->args[dS->curarg+argoff];
        arg         = entry->arg;
    }
    else
    {
        arg             = entry->arg;
        while (arg->next != 0)
            arg         = arg->next;
        arg->next       = &dE->args[dS->curarg+argoff];
        arg             = arg->next;
    }
    arg->value.fst      = &dS->argv[dS->curarg+argoff][off];
    arg->value.lst      = dS->argv[dS->curarg+argoff] + strlen(dS->argv[dS->curarg]);
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
    regfree(&dS->re_optref);
}

static int docopt_parse_state_init(struct docopt_parse_state* dS, const char* doc, int argc, char **argv, char* version, unsigned int flags)
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
    dS->entries->version            = version;
    dS->entries->flags              = flags;
    dS->entries->nentries           = 2;
    dS->entries->curentry           = 1;
    if (!(dS->entries->entries = (struct docopt_entry*)calloc(sizeof(struct docopt_entry), dS->entries->nentries))) goto docoptFail;
    if (!(dS->entries->args = (struct docopt_arg*)calloc(sizeof(struct docopt_arg), argc))) goto docoptFail;
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
    if (regcomp(&dS->re_option_section_start, DOCOPT_RE_OPTION_SECTION_START, REG_EXTENDED | REG_ICASE)) goto docoptFail;
    if (regcomp(&dS->re_option_statement_start, DOCOPT_RE_OPTION_STATEMENT_START, REG_EXTENDED)) goto docoptFail;
    if (regcomp(&dS->re_naked_shorts, "^" DOCOPT_RE_NAKED_SHORTS, REG_EXTENDED)) goto docoptFail;
    if (regcomp(&dS->re_optref, "^" DOCOPT_RE_OPTREF, REG_EXTENDED)) goto docoptFail;

    return 0;
docoptFail:
    return 1;
}

/* After this point it is illegal to declare docopt_term*, docopt_entry*, or docopt_arg*.*/

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
    if (regexec(start, fst, DOCOPT_NUM_PMATCHES, &docopt_pmatches[0], 0))
        return 1;
    if (docopt_pmatches[pidx].rm_so < 0) return 1;
    sec->fst    = docopt_pmatches[pidx].rm_eo + fst;
    if (regexec(stop, sec->fst, DOCOPT_NUM_PMATCHES, &docopt_pmatches[0], 0)) return 1;
    if (docopt_pmatches[qidx].rm_so < 0) return 1;
    sec->lst    = docopt_pmatches[qidx].rm_so + sec->fst;
    return 0;
}

static int docopt_parse_options(struct docopt_parse_state* dS)
{
    struct docopt_str optstmt   = { 0 };
    struct docopt_entry option  = { };
    int which_first             = 0;
    char unarged_shorts[]       = DOCOPT_RE_ALL_SHORTS_CHARS;
    char re_arged_shorts[256]   = { 0 };
    char short_name[3]          = "- ";
    struct docopt_str sname     = { &short_name[0], &short_name[2] };
    int options                 = 0;
    int options_x               = 0;
    int options_s               = 0;
    int sterm                   = 0;
    int lterm                   = 0;

    if (docopt_get_section(dS, &dS->re_option_section_start, &dS->re_option_section_start, dS->usage_section.lst, &dS->option_section, 3, 0)) return 0;
    dS->option_section.lst  = dS->entries->docstr.lst;
    optstmt.fst                 = dS->option_section.fst;

    options_x                   = docopt_new_term(dS, DOCOPT_TYPE_EXPR);
    docopt_fetch_term(dS, options)->optional = 1;
    docopt_fetch_term(dS, options)->repeat = 1;
    while (!docopt_get_section(dS, &dS->re_option_statement_start, &dS->re_option_statement_start, optstmt.fst, &optstmt, 3, 0))
    {
        sterm           = -1;
        lterm           = -1;
        which_first     = 0;
        memset(&option, 0x0, sizeof(option));
        if (optstmt.fst >= dS->option_section.lst) break;
        docopt_eat_ws(&optstmt);
        for (int X = 0; X < 2; ++X)
        {
            if ((which_first != 1) && regexec(&dS->re_optshort, optstmt.fst, DOCOPT_NUM_PMATCHES, &docopt_pmatches[0], 0) == 0)
            {
                options_s   = docopt_new_term(dS, DOCOPT_TYPE_SEQ);
                sterm       = docopt_new_term(dS, DOCOPT_TYPE_SHORT);
                docopt_fetch_term(dS, sterm)->length = 1;
                which_first = 1;
                sprintf(option.short_name, "-%c", optstmt.fst[1]);
                if (docopt_pmatches[4].rm_so >= 0)
                {
                    option.command.fst  = optstmt.fst + docopt_pmatches[4].rm_so;
                    option.command.lst  = optstmt.fst + docopt_pmatches[4].rm_eo;
                }
                docopt_fetch_term(dS, options_s)->length = dS->curterm - options_s;
            }
            else if ((which_first != 2) && regexec(&dS->re_long, optstmt.fst, DOCOPT_NUM_PMATCHES, &docopt_pmatches[0], 0) == 0)
            {
                which_first = 2;
                options_s       = docopt_new_term(dS, DOCOPT_TYPE_SEQ);
                lterm           = docopt_new_term(dS, DOCOPT_TYPE_LONG);
                docopt_fetch_term(dS, lterm)->length = 1;
                option.name.fst = optstmt.fst;
                option.name.lst = optstmt.fst + docopt_pmatches[2].rm_eo;
                if (docopt_pmatches[4].rm_so >= 0)
                {
                    option.command.fst  = optstmt.fst + docopt_pmatches[4].rm_so;
                    option.command.lst  = optstmt.fst + docopt_pmatches[4].rm_eo;
                }
                docopt_fetch_term(dS, options_s)->length = dS->curterm - options_s;
            }
            if (X == 0) optstmt.fst += docopt_pmatches[0].rm_eo + 1; docopt_eat_ws(&optstmt);
        }
        if (which_first == 0)
        {
            dS->entries->error  = DOCOPT_BAD_DEFAULTS;
            return 1;
        }
        if (regexec(&dS->re_default, optstmt.fst, DOCOPT_NUM_PMATCHES, &docopt_pmatches[0], 0)) goto docoptAddOption;
        if ((optstmt.fst + docopt_pmatches[0].rm_eo) > optstmt.lst) goto docoptAddOption;
        option.defvalue.fst     = optstmt.fst + docopt_pmatches[2].rm_so;
        option.defvalue.lst     = optstmt.fst + docopt_pmatches[2].rm_eo;
docoptAddOption:
        if (lterm >= 0) docopt_fetch_term(dS, lterm)->entry = docopt_entries_insert_entry(dS->entries, &option);
        if (sterm >= 0) docopt_fetch_term(dS, sterm)->entry = docopt_entries_insert_entry(dS->entries, &option);
    }
    docopt_fetch_term(dS, options_x)->length = dS->curterm - options_x;
    for (int i = 0, idx = 0, jdx = 0, entry; i < strlen(unarged_shorts); ++i)
    {
        short_name[1]   = unarged_shorts[i];
        if (((entry = docopt_entries_get_entry(dS->entries, &sname)) < 0) ||
            !docopt_str_len(&docopt_entries_deref(dS->entries, entry)->command))
            dS->unarged_shorts[idx++]   = unarged_shorts[i];
        else
            dS->arged_shorts[jdx++]     = short_name[1];
    }
    sprintf(&re_arged_shorts[0], "^" DOCOPT_RE_ARGED_SHORTS, dS->unarged_shorts, dS->arged_shorts);
    if (regcomp(&dS->re_arged_shorts, re_arged_shorts, REG_EXTENDED))
    {
        dS->entries->error  = DOCOPT_BAD_ARGED_SHORTS;
        return 1;
    }
    return 0;
}

static int docopt_parse_seq(struct docopt_parse_state* dS, struct docopt_str* usestmt);
static int docopt_parse_atom(struct docopt_parse_state* dS, struct docopt_str* usestmt);

static int docopt_parse_expr(struct docopt_parse_state* dS, struct docopt_str* usestmt)
{
    struct docopt_str olduse    = *usestmt;
    int hterm                   = docopt_new_term(dS, DOCOPT_TYPE_EXPR);
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
    docopt_fetch_term(dS, hterm)->length = dS->curterm - hterm;
    return 1;
}

int docopt_parse_seq(struct docopt_parse_state* dS, struct docopt_str* usestmt)
{
    struct docopt_str olduse    = *usestmt;
    int hterm                   = docopt_new_term(dS, DOCOPT_TYPE_SEQ);
    int atom                    = -1;
    while ((usestmt->fst < usestmt->lst) && (atom = docopt_parse_atom(dS, usestmt)))
    {
        docopt_eat_ws(usestmt);
        if (strncmp(DOCOPT_REPEAT, usestmt->fst, strlen(DOCOPT_REPEAT)) == 0)
        {
            docopt_fetch_term(dS, atom)->repeat = 1;
            usestmt->fst        += 3;
        }
    }
    if ((hterm + 1) == dS->curterm)
    {
        *usestmt        = olduse;
        dS->curterm     = hterm;
        return 0;
    }
    docopt_fetch_term(dS, hterm)->length = dS->curterm - hterm;
    return 1;
}

int docopt_parse_atom(struct docopt_parse_state* dS, struct docopt_str* usestmt)
{
    struct docopt_str olduse    = *usestmt;
    struct docopt_entry entry   = { };
    int hterm                   = -1;
    int sterm                   = 0;

    if (usestmt->fst >= usestmt->lst) goto docoptFail;

    if (strncmp(dS->pname.fst, usestmt->fst, docopt_str_len(&dS->pname)) == 0)
    {
        hterm           = docopt_new_term(dS, DOCOPT_TYPE_PNAME);
        usestmt->fst    += docopt_str_len(&dS->pname);
        entry.name      = dS->pname;
        docopt_fetch_term(dS, hterm)->entry = docopt_entries_insert_entry(dS->entries, &entry);
    }
    else if (!regexec(&dS->re_optref, usestmt->fst, DOCOPT_NUM_PMATCHES, &docopt_pmatches[0], 0))
    {
        hterm           = docopt_new_term(dS, DOCOPT_TYPE_OPTREF);
        usestmt->fst    += docopt_pmatches[0].rm_eo;
    }
    else if (strchr("[(", usestmt->fst[0]) != 0)
    {
        hterm   = dS->curterm;
        ++usestmt->fst;
        if (!docopt_parse_expr(dS, usestmt)) goto docoptFail;
        docopt_eat_ws(usestmt);
        if ((olduse.fst[0]+1) != usestmt->fst[0]) goto docoptFail;
        ++usestmt->fst;
        docopt_fetch_term(dS, hterm)->optional = (olduse.fst[0] == '[');
    }
    else if (!regexec(&dS->re_long, usestmt->fst, DOCOPT_NUM_PMATCHES, &docopt_pmatches[0], 0))
    {
        hterm           = docopt_new_term(dS, DOCOPT_TYPE_LONG);
        entry.name.fst  = usestmt->fst;
        entry.name.lst  = usestmt->fst + docopt_pmatches[2].rm_eo;
        if (docopt_pmatches[4].rm_so >= 0)
        {
            entry.command.fst  = usestmt->fst + docopt_pmatches[4].rm_so;
            entry.command.lst  = usestmt->fst + docopt_pmatches[4].rm_eo;
        }
        docopt_fetch_term(dS, hterm)->entry = docopt_entries_insert_entry(dS->entries, &entry);
        usestmt->fst    += docopt_pmatches[0].rm_eo;
    }
    else if ((dS->arged_shorts[0] && !regexec(&dS->re_arged_shorts, usestmt->fst, DOCOPT_NUM_PMATCHES, &docopt_pmatches[0], 0)) ||
        !regexec(&dS->re_naked_shorts, usestmt->fst, DOCOPT_NUM_PMATCHES, &docopt_pmatches[0], 0))
    {
        hterm           = docopt_new_term(dS, DOCOPT_TYPE_SHORTS);
        for (int h = docopt_pmatches[3].rm_so, he = docopt_pmatches[3].rm_eo; h < he; ++h)
        {
            sprintf(&entry.short_name[0], "-%c", usestmt->fst[h]);
            sterm           = docopt_new_term(dS, DOCOPT_TYPE_SHORT);
            docopt_fetch_term(dS, sterm)->length = 1;
            if (((h+1) == docopt_pmatches[3].rm_eo) && (docopt_pmatches[5].rm_so > 0))
            {
                entry.command.fst   = usestmt->fst + docopt_pmatches[5].rm_so;
                entry.command.lst   = usestmt->fst + docopt_pmatches[5].rm_eo;
            }
            docopt_fetch_term(dS, sterm)->entry = docopt_entries_insert_entry(dS->entries, &entry);
        }
        usestmt->fst    += docopt_pmatches[0].rm_eo;
    }
    else if (!regexec(&dS->re_argument, usestmt->fst, DOCOPT_NUM_PMATCHES, &docopt_pmatches[0], 0))
    {
        hterm           = docopt_new_term(dS, DOCOPT_TYPE_ARGUMENT);
        entry.name.fst  = usestmt->fst;
        entry.name.lst  = usestmt->fst + docopt_pmatches[2].rm_eo;
        docopt_fetch_term(dS, hterm)->entry = docopt_entries_insert_entry(dS->entries, &entry);
        usestmt->fst    += docopt_pmatches[0].rm_eo;
    }
    else
    {
        hterm           = docopt_new_term(dS, DOCOPT_TYPE_COMMAND);
        while (
            (usestmt->fst < usestmt->lst) &&
            (strncmp(DOCOPT_REPEAT, usestmt->fst, strlen(DOCOPT_REPEAT)) != 0) &&
            (strchr("()[]|", usestmt->fst[0]) == 0) &&
            !isspace(usestmt->fst[0]) &&
            1)
            ++usestmt->fst;
        if (olduse.fst == usestmt->fst) goto docoptFail;
        entry.name.fst  = olduse.fst;
        entry.name.lst  = usestmt->fst;
        docopt_fetch_term(dS, hterm)->entry = docopt_entries_insert_entry(dS->entries, &entry);
    }

    if (olduse.fst == usestmt->fst) goto docoptFail;
    docopt_fetch_term(dS, hterm)->length    = dS->curterm - hterm;
    return hterm;
docoptFail:
    *usestmt        = olduse;
    dS->curterm     = hterm;
    return 0;
}

static int docopt_parse_usage(struct docopt_parse_state* dS)
{
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

    dS->usage_term  = docopt_new_term(dS, DOCOPT_TYPE_EXPR);
    usestmt.fst     = dS->usage_section.fst;
    while (!docopt_get_section(dS, &dS->re_usage_statement_start, &dS->re_usage_statement_start, usestmt.fst, &usestmt, 3, 0))
    {
        if (usestmt.fst >= dS->usage_section.lst) break;
        usestmt.lst     = usestmt.lst > dS->usage_section.lst ? dS->usage_section.lst : usestmt.lst;
        usage_section_s = docopt_new_term(dS, DOCOPT_TYPE_SEQ);
        docopt_fetch_term(dS, usage_section_s)->eat_all = 1;
        if (!docopt_parse_expr(dS, &usestmt))
        {
            dS->entries->error   = DOCOPT_BAD_USAGE_STATEMENT;
            return 1;
        }
        docopt_fetch_term(dS, usage_section_s)->length = dS->curterm - usage_section_s;
    }
    docopt_fetch_term(dS, dS->usage_term)->length = dS->curterm - dS->usage_term;

    return 0;
}

static int docopt_parse_simple_backend(struct docopt_parse_state* dS, int hterm)
{
    static int indent = 0;
    const int indentsc = 2;
    int result                  = 0;
    int curarg                  = dS->curarg;
    int arglen                  = 0;
    int cmdlen                  = 0;
    struct docopt_term* term    = docopt_fetch_term(dS, hterm);
    struct docopt_entries dE    = *dS->entries;
    struct docopt_entries *oldE = dS->entries;
    dE.nentries                 = dS->entries->nentries;
    dE.entries                  = (struct docopt_entry*)calloc(sizeof(struct docopt_entry), dE.nentries);
    memcpy(dE.entries, dS->entries->entries, sizeof(struct docopt_entry) * dE.nentries);
    struct docopt_entry* entry  = docopt_entries_deref(&dE, term->entry);
    dS->entries                 = &dE;

    switch (term->type)
    {
    case DOCOPT_TYPE_BAD        :
        fprintf(dS->log, "%*s%s\n", indent+indentsc, "", "BAD");
        break;
docoptParseLong:
    case DOCOPT_TYPE_LONG       :
        if (dS->curarg >= dS->argc) break;
        if (strncmp(entry->name.fst, dS->argv[dS->curarg], docopt_str_len(&entry->name)))
        {
            if (entry->short_name[0] && (term->type == DOCOPT_TYPE_LONG))
                goto docoptParseShort;
            break;
        }
        fprintf(dS->log, "%*sLONG %.*s\n", indent+indentsc, "", DOCOPT_PR((&entry->name)));
        arglen  = strlen(dS->argv[dS->curarg]);
        cmdlen  = docopt_str_len(&entry->name);
        if (entry->command.fst)
        {
            fprintf(dS->log, "NEEDS COMMAND\n");
            if ((cmdlen < arglen) && (dS->argv[dS->curarg][cmdlen] == '='))
            {
                docopt_add_arg_list(dS, &dE, entry, 0, cmdlen+1, term->repeat);
            }
            else if ((dS->curarg + 1) < dS->argc)
            {
                docopt_add_arg_list(dS, &dE, entry, 1, 0, term->repeat);
                ++dS->curarg;
            }
            else break;
        }
        else docopt_add_arg_list(dS, &dE, entry, 0, 0, term->repeat);
        ++dS->curarg;
        result = 1;
        break;
    case DOCOPT_TYPE_SHORTS     :
        if (dS->curarg >= dS->argc) break;
        if (dS->argv[dS->curarg][0] != '-') break;
        if (dS->argv[dS->curarg][1] == '-') break;
        fprintf(dS->log, "%*s%s(%d)\n", indent+indentsc, "", "SHORTS", docopt_fetch_term(dS, hterm)->length);
        ++indent;
        dS->curargoff = 0;
        for (int i = 1; i < term->length; i += docopt_fetch_term(dS, hterm + i)->length)
            if (docopt_parse_simple_backend(dS, hterm+i))
                result = 1;
        dS->curargoff = 0;
        --indent;
        break;
docoptParseShort:
    case DOCOPT_TYPE_SHORT      :
        if (dS->curarg >= dS->argc) break;
        if (dS->argv[dS->curarg][0] != '-') break;
        if (dS->argv[dS->curarg][1] == '-') break;
        if ((entry->short_name[1] != dS->argv[dS->curarg][dS->curargoff+1]) &&
            strncmp(entry->short_name, dS->argv[dS->curarg], 2))
        {
            if (entry->name.fst && (term->type == DOCOPT_TYPE_SHORT))
                goto docoptParseLong;
            break;
        }
        fprintf(dS->log, "%*sSHORT %.*s == %s\n", indent+indentsc, "", 2, entry->short_name, dS->argv[dS->curarg]);
        if (entry->command.fst)
        {
            if (strlen(dS->argv[dS->curarg]) == (dS->curargoff+2))
            {
                docopt_add_arg_list(dS, &dE, entry, 0, dS->curargoff+2, term->repeat);
                dS->curarg      += 2;
            }
            else
            {
                docopt_add_arg_list(dS, &dE, entry, 0, 2, term->repeat);
                ++dS->curarg;
            }
            dS->curargoff   = 0;
        }
        else
        {
            docopt_add_arg_list(dS, &dE, entry, 0, 0, term->repeat);
            if ((dS->curargoff+2) >= strlen(dS->argv[dS->curarg]))
            {
                dS->curargoff   = 0;
                ++dS->curarg;
            }
            else ++dS->curargoff;
        }
        result = 1;
        break;
    case DOCOPT_TYPE_OPTREF     :
        fprintf(dS->log, "%*s%s\n", indent+indentsc, "", "[options]");
        ++indent;
        docopt_parse_simple_backend(dS, 0);
        result = 1;
        --indent;
        break;
    case DOCOPT_TYPE_PNAME      :
        fprintf(dS->log, "%*s%.*s\n", indent+indentsc, "", DOCOPT_PR((&entry->name)));
        result = 1;
        break;
    case DOCOPT_TYPE_COMMAND    :
        if (dS->curarg >= dS->argc) break;
        fprintf(dS->log, "%*s%.*s\n", indent+indentsc, "", DOCOPT_PR((&entry->name)));
        docopt_add_arg_list(dS, &dE, entry, 0, 0, term->repeat);
        ++dS->curarg;
        result = 1;
        break;
    case DOCOPT_TYPE_ARGUMENT   :
        if (dS->curarg >= dS->argc) break;
        if (!dS->positional && (strncmp(dS->argv[dS->curarg], "-", 1) == 0)) break;
        fprintf(dS->log, "%*s%.*s\n", indent+indentsc, "", DOCOPT_PR((&entry->name)));
        docopt_add_arg_list(dS, &dE, entry, 0, 0, term->repeat);
        ++dS->curarg;
        result = 1;
        break;
    case DOCOPT_TYPE_SEQ        :
        fprintf(dS->log, "%*s%s%s(%d)\n", indent+indentsc, "", "SEQ", term->optional ? "?" : "", term->length);
        ++indent;
        result = 1;
        for (int i = 1; i < term->length; i += docopt_fetch_term(dS, hterm + i)->length)
            if (!docopt_parse_simple_backend(dS, hterm+i))
            {
                result = 0;
                break;
            }
        if (term->eat_all && (dS->curarg < dS->argc)) result = 0;
        --indent;
        break;
    case DOCOPT_TYPE_EXPR       :
        fprintf(dS->log, "%*s%s%s(%d)\n", indent+indentsc, "", "EXPR", term->optional ? "?" : "", term->length);
        ++indent;
        for (int i = 1; i < term->length; i += docopt_fetch_term(dS, hterm + i)->length)
            if (docopt_parse_simple_backend(dS, hterm+i))
            {
                fprintf(dS->log, "%*s%s\n", (indent+1)*indentsc, "", "SUCCESS.");
                result = 1;
                break;
            }
        --indent;
        break;
    case DOCOPT_TYPE_MAX        :
        fprintf(dS->log, "%*s%s\n", indent+indentsc, "", "MAX");
        break;
    }

    if (!result)
    {
        dS->curarg  = curarg;
    }
    else
    {
        for (int i = 0; i < dE.nentries; ++i)
        {
            // XXX: unification is right-destructive.
            oldE->entries[i].arg = dE.entries[i].arg;
        }
    }

    if (term->repeat && (curarg < dS->curarg))
    {
        assert(curarg < dS->argc);
        docopt_parse_simple_backend(dS, hterm);
    }

    dS->entries = oldE;
    free(dE.entries);

    return result || term->optional;
}

static int docopt_parse_args(struct docopt_parse_state* dS)
{
    if (dS->entries->flags | DOCOPT_SIMPLE_BACKEND)
    {
        return !docopt_parse_simple_backend(dS, dS->usage_term);
    }
    return 1;
}

void docopt_print_help(struct docopt_parse_state* dS, char* version, int moreInfo)
{
    dS->entries->error  = DOCOPT_PRINTED_HELP;
    if (!version)
    {
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
    else fprintf(stderr, "%s", version);
}

docopt_t docopt(const char* doc, int argc, char** argv, char* version, unsigned int flags)
{
    struct docopt_parse_state   dS  = { 0 };
    struct docopt_entries*      dE  = 0;
    if (docopt_parse_state_init(&dS, doc, argc, argv, version, flags)) goto docoptFail;
    if (docopt_get_section(&dS, &dS.re_usage_section_start, &dS.re_option_section_start, dS.entries->docstr.fst, &dS.usage_section, 0, 0)) goto docoptFail;
    if (regexec(&dS.re_pname, dS.usage_section.fst, DOCOPT_NUM_PMATCHES, &docopt_pmatches[0], 0)) return 0;
    dS.pname.fst    = docopt_pmatches[2].rm_so + dS.usage_section.fst;
    dS.pname.lst    = docopt_pmatches[2].rm_eo + dS.usage_section.fst;
    if (docopt_parse_options(&dS)) goto docoptFail;
    if (docopt_parse_usage(&dS)) goto docoptFail;
    if (argc && docopt_parse_args(&dS))
    {
        goto docoptFail;
    }

    dE      = dS.entries;

    for (int i = 0; (i < argc) && !(flags & DOCOPT_NO_HELP); ++i)
        if (!strncmp(argv[i], "--help", 6) || !strncmp(argv[i], "-h", 2))
        {
            docopt_print_help(&dS, version, 1);
            break;
        }
    if (argc == 0) docopt_print_help(&dS, version, 0);

docoptFail:
    if (!dE) docopt_print_help(&dS, version, 0);
    docopt_parse_state_free(&dS);
    return dE;
}

int docopt_error(docopt_t doc)
{
    return ((struct docopt_entries*)doc)->error;
}

docopt_str_t* docopt_get(docopt_t doc, const char* cmd, int idx)
{
    struct docopt_entries* dE   = doc;
    struct docopt_str str       = { .fst = cmd, .lst = cmd + strlen(cmd) };
    struct docopt_arg* arg      = 0;
    int hentry  = docopt_entries_get_entry(dE, &str);
    if (hentry < 0) return 0;
    arg = docopt_entries_deref(dE, hentry)->arg;
    while ((idx > 0) && arg)
    {
        --idx;
        arg = arg->next;
    }
    if (arg) return &arg->value;
    return 0;
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

#endif/*DOCOPT_C*/
