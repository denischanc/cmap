%{

#include "cmap-scanner.h"
#include "cmap-parser.h"
#include "cmap-parser-util.h"
#include "cmap-log.h"
#include "cmap-common.h"
#include "cmap-string.h"
#include "cmap-kernel.h"

static void cmap_parser_error(yyscan_t yyscanner, CMAP_PROC_CTX * proc_ctx,
  const char * msg);

int cmap_parser_debug = 1;

%}

%define api.pure full
%define api.push-pull both
/*%define parse.trace*/

%param {yyscan_t yyscanner}
%parse-param {CMAP_PROC_CTX * proc_ctx}

%union
{
  char * name;
  CMAP_MAP * map;
  CMAP_LIST * list;
  CMAP_STRING * string;
}

%token ERROR FUNCTION
%token<name> NAME LOCAL NEW RETURN NULL_PTR
%token<string> STRING

%type<map> cmap creator path process process_chain new function
%type<name> aisle name
%type<list> args args_name args_name_cmap

%%

/*******************************************************************************
*******************************************************************************/

start: instructions { proc_ctx -> ret = NULL; };

instructions: instruction ';'
| instructions instruction ';';

/*******************************************************************************
*******************************************************************************/

name: NAME | LOCAL | NEW | RETURN;

/*******************************************************************************
*******************************************************************************/

instruction: LOCAL name '=' cmap
{
  CMAP_KERNEL_FREE($1);
  cmap_parser_util_public.set_local(proc_ctx, $2, $4);
}
| name '=' cmap { cmap_parser_util_public.set_global(proc_ctx, $1, $3); }
| path '.' name '=' cmap { cmap_parser_util_public.set_path($1, $3, $5); }
| process
| RETURN cmap { CMAP_KERNEL_FREE($1); proc_ctx -> ret = $2; YYACCEPT; };

/*******************************************************************************
*******************************************************************************/

aisle: { $$ = NULL; } | '<' name '>' { $$ = $2; };

creator: '{' args_name_cmap '}' aisle
{
  $$ = cmap_parser_util_public.map_args($2, proc_ctx, $4);
}
| '[' args ']' aisle
{
  $$ = cmap_parser_util_public.list_args($2, proc_ctx, $4);
}
| STRING aisle { $$ = cmap_parser_util_public.string($1, proc_ctx, $2); };

/*******************************************************************************
*******************************************************************************/

path: name { $$ = cmap_parser_util_public.name(proc_ctx, $1); }
| path '.' name { $$ = cmap_parser_util_public.path($1, $3); };

/*******************************************************************************
*******************************************************************************/

args: { $$ = cmap_parser_util_public.args_empty(proc_ctx); }
| cmap { $$ = cmap_parser_util_public.args(proc_ctx, $1); }
| args ',' cmap { $$ = cmap_parser_util_public.args_push($1, $3); };

/*******************************************************************************
*******************************************************************************/

args_name: { $$ = cmap_parser_util_public.args_empty(proc_ctx); }
| name { $$ = cmap_parser_util_public.args_name(proc_ctx, $1); }
| args_name ',' name
{
  $$ = cmap_parser_util_public.args_name_push(proc_ctx, $1, $3);
};

/*******************************************************************************
*******************************************************************************/

args_name_cmap: { $$ = cmap_parser_util_public.args_empty(proc_ctx); }
| name ':' cmap { $$ = cmap_parser_util_public.args_map(proc_ctx, $1, $3); }
| args_name_cmap ',' name ':' cmap
{
  $$ = cmap_parser_util_public.args_map_push(proc_ctx, $1, $3, $5);
};

/*******************************************************************************
*******************************************************************************/

cmap: creator | path | process | new | function
| NULL_PTR { CMAP_KERNEL_FREE($1); $$ = NULL; };

/*******************************************************************************
*******************************************************************************/

process: name '(' args ')'
{
  $$ = cmap_parser_util_public.process(proc_ctx, NULL, $1, $3);
}
| path '.' name '(' args ')'
{
  $$ = cmap_parser_util_public.process(proc_ctx, $1, $3, $5);
}
| process_chain '.' name '(' args ')'
{
  $$ = cmap_parser_util_public.process(proc_ctx, $1, $3, $5);
};

process_chain: process
| process_chain '.' name { $$ = cmap_parser_util_public.path($1, $3); };

/*******************************************************************************
*******************************************************************************/

new: NEW name '(' args ')' aisle
{
  CMAP_KERNEL_FREE($1);
  $$ = cmap_parser_util_public.new(NULL, $2, $4, proc_ctx, $6);
}
| NEW path '.' name '(' args ')' aisle
{
  CMAP_KERNEL_FREE($1);
  $$ = cmap_parser_util_public.new($2, $4, $6, proc_ctx, $8);
}
| NEW process_chain '.' name '(' args ')' aisle
{
  CMAP_KERNEL_FREE($1);
  $$ = cmap_parser_util_public.new($2, $4, $6, proc_ctx, $8);
};

/*******************************************************************************
*******************************************************************************/

function: FUNCTION '(' args_name ')' aisle STRING
{
  $$ = cmap_parser_util_public.function(proc_ctx, $5, $3, $6);
};

/*******************************************************************************
*******************************************************************************/

%%

static void cmap_parser_error(yyscan_t yyscanner, CMAP_PROC_CTX * proc_ctx,
  const char * msg)
{
  cmap_log_public.error("[%d:%d] %s", cmap_parser_get_lineno(yyscanner),
    cmap_parser_get_column(yyscanner), msg);
}
