DOCOPTC
=======

Docoptc is a [docopt](http://docopt.org) *variant*, not a port.


#### Docoptc design

The design of docoptc contains two broad phases:

  1. document string parsing; and,
  2. argument parsing.

The first phase is (fairly) well-defined by the docopt.org website. The second phase is undefined, other than reference code. Docoptc has no intention of porting the second (undefined) phase and can be considered a mere *variant*, and not a fully functioning port. However, the design of docoptc is such that more argument-parsing backends can be integrated---possibly even a docopt.org-compliant version.

##### Document string parsing

Document string parsing occurs in thre phases:

  1. Usage section discovery;
  2. Option section(s) discovery;
  3. Section parsing.

There must be one, and only one, usage section. A usage section is defined as the first section of the document string. A usage begins immediately after the (case insensitive) word `usage:`, and ends either with the end of the document string, the (case insensitive) word `options:`, or a sequence of white-space containing two return carriages (\n).

There can be zero or more options sections. An option section *must* start after another section (usage or options), and begins immediately after the (case insensitive) word `options:`. An options section ends either with the end of the document string, the (case insensitive) word `options:`.

Sections are parsed according to their type (usage; options). The options section(s) *must* be parsed first; the usage section *must* be parsed last.

An options section is divided into a sequence of one, or more, option statements. An option statement begins with a return carriage (\n), zero or more non-return carriage white-space characters, and a hyphen (-). An option statement ends with the end of its options section, or the beginning of the next option statement.

An option statement begins with a SHORT, an optional comma (,), and an optional LONG; alternately, it may begin with a LONG, an optional comma (,), and an optional SHORT. Before the end of the option statement a DEFAULT may occur. If both a SHORT and a LONG are defined, the SHORT and LONG are grouped as an alias: in any situation in which the SHORT is required, the LONG may be substituted, and vice versa. If either the SHORT or the LONG takes a command, then both take a command, whether or not specified. If the SHORT or LONG takes a command, then the SHORT is marked as an ARGED-SHORT.

The usage section uses the following (rough) grammar:

    USAGE-SECTION ::= EXPR
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

##### Arguments parsing

Currently, docoptc uses the AST defined by the parsing of the usage- and options- sections to form a parsing-expression-grammar. **This parsing is known to be non-compliant with docopt.org**. This grammar is interpreted with the following (rough) semantics:

              [| EXPR |] === one of the branches must succeed
               [| SEQ |] === each element of the sequence must succeed, in order
              [| ATOM |] === the atom must succeed
        [| ATOM `...` |] === as atom, but one-or-more times
      [| `[` EXPR `]` |] === as [|EXPR|], but success always occurs;
                             combined with `...`, care must be made with *progress*
      [| `(` EXPR `)` |] === [| EXPR |]
             [| SHORT |] === must parse a short; see note about "stacked shorts"
    [| SHORT ARGUMENT |] === must parse a short, followed by an argument;
                             see note about "stacked shorts"
              [| LONG |] === must parse a long
     [| LONG ARGUMENT |] === must parse a long, followed by an argument
          [| ARGUMENT |] === must parse an argument
           [| COMMAND |] === must parse a command

"Stacked shorts" allow a (predefined) sequence of shorts to be parsed. It essentially adds an EXPR-like branch, except that some (non-contiguous) subset of the short-sequence must be parsed.

