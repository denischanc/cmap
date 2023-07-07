%{

#include "cmap-scanner.h"
#include "cmap-parser.h"
#include "cmap-parser-util.h"
#include "cmap-parser-part.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int cmap_parser_debug = 1;

static void cmap_parser_error(yyscan_t yyscanner, const char * msg);

%}

%define api.pure full
/*%define parse.trace*/

%param {yyscan_t yyscanner}

%union
{
  char * name;
}

%token INCLUDE FUNCTION_C STATIC_FUNCTION_C LOCAL NULL_PTR RETURN ERROR
%token<name> STRING NAME INT

%type<name> name cmap aisle arg_names_cmap creator path args
%type<name> process process_chain

%%

/*******************************************************************************
*******************************************************************************/

start: includes function_cs;

includes:
| includes include;

function_cs:
| function_cs function_c;

/*******************************************************************************
*******************************************************************************/

include: INCLUDE STRING { cmap_parser_util_public.include($2); };

/*******************************************************************************
*******************************************************************************/

function_c: FUNCTION_C '(' NAME ')' '{' instructions '}'
{
  cmap_parser_util_public.function_c($3, (1 == 0));
}
| STATIC_FUNCTION_C '(' NAME ')' '{' instructions '}'
{
  cmap_parser_util_public.function_c($3, (1 == 1));
};

/*******************************************************************************
*******************************************************************************/

instructions: { cmap_parser_part_public.push_instructions(); }
| instructions instruction ';';

/*******************************************************************************
*******************************************************************************/

instruction: LOCAL name '=' cmap { cmap_parser_util_public.set_local($2, $4); }
| name '=' cmap { cmap_parser_util_public.set_global($1, $3); }
| path '.' name '=' cmap { cmap_parser_util_public.set_path($1, $3, $5); }
| process { free($1); }
| RETURN cmap { cmap_parser_util_public.return_($2); };

/*******************************************************************************
*******************************************************************************/

aisle: { $$ = NULL; } | '/' name '/' { $$ = $2; };

/*******************************************************************************
*******************************************************************************/

creator: '{' arg_names_cmap '}' aisle
{
  $$ = cmap_parser_util_public.map_args($2, $4);
}
| '[' args ']' aisle
{
  $$ = cmap_parser_util_public.list_args($2, $4);
}
| STRING aisle { $$ = cmap_parser_util_public.string($1, $2); }
| INT aisle { $$ = cmap_parser_util_public.int_($1, $2); };

/*******************************************************************************
*******************************************************************************/

name: NAME;

path: name { $$ = cmap_parser_util_public.name($1); }
| path '.' name { $$ = cmap_parser_util_public.path($1, $3); }
| creator;

cmap: path | process | NULL_PTR { $$ = strdup("NULL"); };

/*******************************************************************************
*******************************************************************************/

args: { $$ = NULL; }
| cmap
| args ',' cmap { $$ = cmap_parser_util_public.args_push($1, $3); };

/*******************************************************************************
*******************************************************************************/

arg_names_cmap: { $$ = NULL; }
| name ':' cmap { $$ = cmap_parser_util_public.args_map($1, $3); }
| arg_names_cmap ',' name ':' cmap
{
  $$ = cmap_parser_util_public.args_map_push($1, $3, $5);
};

/*******************************************************************************
*******************************************************************************/

process: name '(' args ')'
{
  $$ = cmap_parser_util_public.process(NULL, $1, $3);
}
| path '.' name '(' args ')'
{
  $$ = cmap_parser_util_public.process($1, $3, $5);
}
| process_chain '.' name '(' args ')'
{
  $$ = cmap_parser_util_public.process($1, $3, $5);
};

process_chain: process
| process_chain '.' name { $$ = cmap_parser_util_public.path($1, $3); };

/*******************************************************************************
*******************************************************************************/

%%

static void cmap_parser_error(yyscan_t yyscanner, const char * msg)
{
  fprintf(stderr, "[%d:%d] %s", cmap_parser_get_lineno(yyscanner),
    cmap_parser_get_column(yyscanner), msg);
}
