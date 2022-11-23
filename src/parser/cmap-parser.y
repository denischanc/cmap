%{

#include "cmap-scanner.h"
#include "cmap-parser.h"
#include "cmap-parser-util.h"
#include "cmap-log.h"
#include "cmap-common.h"
#include "cmap-string.h"

static void cmap_parser_error(yyscan_t yyscanner, CMAP_PROC_CTX * proc_ctx,
  const char * msg);

%}

%define api.pure full
%define api.push-pull both

%param {yyscan_t yyscanner}
%parse-param {CMAP_PROC_CTX * proc_ctx}

%union
{
  char * name;
  CMAP_MAP * map;
  CMAP_LIST * list;
  CMAP_STRING * string;
}

%token ERROR
%token<name> NAME LOCAL
%token<string> STRING

%type<map> path cmap creator function fn_chain
%type<name> aisle name
%type<list> args args_name_cmap

%%

/*******************************************************************************
*******************************************************************************/

start: instructions;

instructions: instruction ';'
| instructions instruction ';';

/*******************************************************************************
*******************************************************************************/

name: NAME | LOCAL;

/*******************************************************************************
*******************************************************************************/

instruction: LOCAL name '=' cmap
{
  cmap_parser_util_public.set_local(proc_ctx, $2, $4);
}
| name '=' cmap { cmap_parser_util_public.set_global(proc_ctx, $1, $3); }
| path '.' name '=' cmap
{
  cmap_parser_util_public.set_path($1, $3, $5);
}
| function;

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
| STRING aisle
{
  $$ = cmap_parser_util_public.string($1, proc_ctx, $2);
};

/*******************************************************************************
*******************************************************************************/

path: name { $$ = cmap_parser_util_public.name(proc_ctx, $1); }
| path '.' name { $$ = cmap_parser_util_public.path($1, $3); };

/*******************************************************************************
*******************************************************************************/

args_name_cmap: { $$ = cmap_parser_util_public.args_empty(proc_ctx); }
| name ',' cmap { $$ = cmap_parser_util_public.args_map(proc_ctx, $1, $3); }
| args_name_cmap ',' name ',' cmap
{
  $$ = cmap_parser_util_public.args_map_push(proc_ctx, $1, $3, $5);
};

/*******************************************************************************
*******************************************************************************/

args: { $$ = cmap_parser_util_public.args_empty(proc_ctx); }
| cmap { $$ = cmap_parser_util_public.args(proc_ctx, $1); }
| args ',' cmap { $$ = cmap_parser_util_public.args_push($1, $3); };

/*******************************************************************************
*******************************************************************************/

cmap: creator | path | fn_chain;

/*******************************************************************************
*******************************************************************************/

function: name '(' args ')'
{
  $$ = cmap_parser_util_public.process(proc_ctx, NULL, $1, $3);
}
| path '.' name '(' args ')'
{
  $$ = cmap_parser_util_public.process(proc_ctx, $1, $3, $5);
}
| fn_chain '.' name '(' args ')'
{
  $$ = cmap_parser_util_public.process(proc_ctx, $1, $3, $5);
};

fn_chain: function
| fn_chain '.' name { $$ = cmap_parser_util_public.path($1, $3); };

/*******************************************************************************
*******************************************************************************/

%%

static void cmap_parser_error(yyscan_t yyscanner, CMAP_PROC_CTX * proc_ctx,
  const char * msg)
{
  cmap_log_public.error(msg);
}
