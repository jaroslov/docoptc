DOCOPTC
=======

Docoptc is a [docopt](http://docopt.org) *variant*, not a port. However, it can currently consume all the examples given in the python reference implementation.

#### Docoptc building

Docoptc is a single, combined, header and source file. To use as a header file, simply include it as usual:

    #include "docopt.h"

To use it as a source file, define "DOCOPT_C", and include the file:

    #define DOCOPT_C
    #include "docopt.h"

If you feel there is a bug (not unlikely), then debugging can be turned on, as well:

    #define DOCOPT_C
    #define DOCOPT_DEBUG
    #include "docopt.h"

Docoptc is known to compile with [clang](http://clang.llvm.org).

#### Docoptc using

**Put a brief DOCOPT description, here**

#### Docoptc design

The design of docoptc contains two broad phases:

  1. document string parsing; and,
  2. argument parsing.

Both phases are (fairly) well-defined by the docopt.org website. However, the design of docoptc is such that more-than-one argument-parsing backends can be integrated.

##### Document string parsing

Document string parsing occurs in three phases:

  1. Usage section discovery;
  2. Option section(s) discovery;
  3. Section parsing.

There must be one, and only one, usage section. A usage section is defined as the first section of the document string. A usage begins immediately after the (case insensitive) word `usage:`, and ends either with the end of the document string, the (case insensitive) word `options:`, or a sequence of white-space containing two return carriages (\n).

There can be zero or more options sections. An option section *must* start after another section (usage or options), and begins immediately after the (case insensitive) word `options:`. An options section ends either with the end of the document string, or the (case insensitive) word `options:`.

Sections are parsed according to their type (usage; options). The options section(s) *must* be parsed first; the usage section *must* be parsed last.

An options section is divided into a sequence of one, or more, option statements. An option statement begins with a return carriage (\n), zero or more non-return carriage white-space characters, and a hyphen (-). An option statement ends with the end of its options section, or the beginning of the next option statement.

An option statement begins with a SHORT, an optional comma (,), and an optional LONG; alternately, it may begin with a LONG, an optional comma (,), and an optional SHORT. Before the end of the option statement a DEFAULT may occur. If both a SHORT and a LONG are defined, the SHORT and LONG are grouped as an alias: in any situation in which the SHORT is required, the LONG may be substituted, and vice versa. If either the SHORT or the LONG takes a command, then both take a command, whether or not specified. If the SHORT or LONG takes a command, then the SHORT is marked as an ARGED-SHORT. A DEFAULT begins with the word `[default: ` (note the space), and contains any sequence of characters, but `]`, and terminates with `]`. The sequence of characters is the default value.

The usage section uses the following (rough) grammar:

    USAGE-SECTION ::= USAGE-STMT
    USAGE-SECTION ::= USAGE-STMT USAGE-SECTION
       USAGE-STMT ::= EXPR/SEQ/(PNAME ...)
             EXPR ::= SEQ
             EXPR ::= SEQ `|` EXPR
              SEQ ::= ATOM
              SEQ ::= ATOM SEQ
              SEQ ::= ATOM `...`
              SEQ ::= ATOM `...` SEQ
             ATOM ::= `[` EXPR `]`
             ATOM ::= `(` EXPR `)`
             ATOM ::= LONG
             ATOM ::= SHORT
             ATOM ::= ARGUMENT
             ATOM ::= COMMAND
             LONG ::= `--` [a-zA-Z][a-ZA-Z0-9_-]*
             LONG ::= `--` [a-zA-Z][a-ZA-Z0-9_-]* `[= ]` ARGUMENT
            SHORT ::= `-` [a-zA-Z, but not ARGED-SHORT chars][ARGED-SHORT char]?
            SHORT ::= `-` [a-zA-Z, but not ARGED-SHORT chars] `[ ]` ARGUMENT
            SHORT ::= `-` [a-zA-Z, but not ARGED-SHORT chars][ARGED-SHORT char] `[ ]?` ARGUMENT
         ARGUMENT ::= [A-Z]
         ARGUMENT ::= `<` `[^>]+` `>`
          COMMAND ::= not `...`, not whitespace, etc.

The resulting AST defines a grammar for parsing arguments.

##### Arguments parsing

Arguments are first classified as either a LONG, a SHORT, or an ARGUMENT; LONGs and SHORTs are parsed, as above. (The docopt reference refers to this as argument tokenization.) If the ARGUMENT `--` is seen in the arguments, then all subsequent arguments are classified as ARGUMENT.

The final stage is to unify the tokenized arguments against the grammar defined by the parsed document string. This is done by letting the document string grammar be interpreted as a parsing expression grammar.

The option 'options first' enforces that all 'loose' options defined in the options sections are parsed *before* the command section; otherwise, they may occur, interspersed. That is, there are two parsing sequences:

    [ LONG | SHORT ]* [ ARGUMENT ]* [ '--' [ ARGUMENT ]* ];

and:

    [ LONG | SHORT | ARGUMENT ]* [ '--' [ ARGUMENT ]* ].
