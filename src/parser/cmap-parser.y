%{

#include "cmap-scanner.h"
#include "cmap-parser.h"
#include "cmap-parser-util.h"
#include "cmap-log.h"
#include "cmap-common.h"
#include "cmap-string.h"

static void cmap_parser_error(yyscan_t yyscanner, CMAP_MAP * definitions,
  SCANNER_NODE ** chain, const char * msg);

%}

%define api.pure full
%define api.push-pull both

%param {yyscan_t yyscanner}
%parse-param {CMAP_MAP * definitions} {SCANNER_NODE ** chain}

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
  cmap_parser_util_public.set_local(definitions, $2, $4);
}
| name '=' cmap { cmap_parser_util_public.set_global($1, $3); }
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
  $$ = cmap_parser_util_public.map_args($2, $4);
}
| '[' args ']' aisle { $$ = cmap_parser_util_public.list_args($2, $4); }
| STRING aisle { $$ = cmap_parser_util_public.string($1, $2); };

/*******************************************************************************
*******************************************************************************/

path: name { $$ = cmap_parser_util_public.name(definitions, $1); }
| path '.' name { $$ = cmap_parser_util_public.path($1, $3); };

/*******************************************************************************
*******************************************************************************/

args_name_cmap: { $$ = cmap_parser_util_public.args_empty(); }
| name ',' cmap { $$ = cmap_parser_util_public.args_map($1, $3); }
| args_name_cmap ',' name ',' cmap
{
  $$ = cmap_parser_util_public.args_map_push($1, $3, $5);
};

/*******************************************************************************
*******************************************************************************/

args: { $$ = cmap_parser_util_public.args_empty(); }
| cmap { $$ = cmap_parser_util_public.args($1); }
| args ',' cmap { $$ = cmap_parser_util_public.args_push($1, $3); };

/*******************************************************************************
*******************************************************************************/

cmap: creator | path | fn_chain;

/*******************************************************************************
*******************************************************************************/

function: name '(' args ')'
{
  $$ = cmap_parser_util_public.process(definitions, NULL, $1, $3);
}
| path '.' name '(' args ')'
{
  $$ = cmap_parser_util_public.process(definitions, $1, $3, $5);
}
| fn_chain '.' name '(' args ')'
{
  $$ = cmap_parser_util_public.process(definitions, $1, $3, $5);
};

fn_chain: function
| fn_chain '.' name { $$ = cmap_parser_util_public.path($1, $3); };

/*******************************************************************************
*******************************************************************************/

%%

static void cmap_parser_error(yyscan_t yyscanner, CMAP_MAP * definitions,
  SCANNER_NODE ** chain, const char * msg)
{
  cmap_log_public.error(msg);
}
