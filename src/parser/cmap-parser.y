%{

#include "cmap-scanner.h"
#include "cmap-parser.h"
#include "cmap-parser-util.h"
#include "cmap-log.h"
#include "cmap-kernel.h"
#include "cmap-proc-ctx.h"
#include "cmap-cmp.h"

#define proc_ctx cmap_parser_get_extra(yyscanner)

int cmap_parser_debug = 1;

static void cmap_parser_error(yyscan_t yyscanner, const char * msg);

%}

%define api.pure full
%define api.push-pull both
/*%define parse.trace*/

%param {yyscan_t yyscanner}

%union
{
  char * name;
  CMAP_MAP * map;
  CMAP_LIST * list;
  CMAP_STRING * string;
  int64_t int_;
}

%token ERROR LOCAL NEW RETURN NULL_PTR FUNCTION IF ELSE LE GE EQUAL DIFF
%token<name> NAME
%token<string> STRING
%token<int_> INT

%type<map> cmap creator path process process_chain new function
%type<map> comparison comparison_int
%type<name> aisle name
%type<list> args arg_names arg_names_cmap if else

%%

/*******************************************************************************
*******************************************************************************/

start: instructions { proc_ctx -> ret = NULL; }
| comparison { proc_ctx -> ret = $1; };

instructions:
| instructions instruction ';'
| instructions if { cmap_parser_util_public.if_process(proc_ctx, $2); };

/*******************************************************************************
*******************************************************************************/

instruction: LOCAL name '=' cmap
{
  cmap_parser_util_public.set_local(proc_ctx, $2, $4);
}
| name '=' cmap { cmap_parser_util_public.set_global(proc_ctx, $1, $3); }
| path '.' name '=' cmap { cmap_parser_util_public.set_path($1, $3, $5); }
| process
| RETURN cmap { proc_ctx -> ret = $2; YYACCEPT; };

/*******************************************************************************
*******************************************************************************/

aisle: { $$ = NULL; } | '/' name '/' { $$ = $2; };

/*******************************************************************************
*******************************************************************************/

creator: '{' arg_names_cmap '}' aisle
{
  $$ = cmap_parser_util_public.map_args($2, proc_ctx, $4);
}
| '[' args ']' aisle
{
  $$ = cmap_parser_util_public.list_args($2, proc_ctx, $4);
}
| STRING aisle { $$ = cmap_parser_util_public.string($1, proc_ctx, $2); }
| INT aisle { $$ = cmap_parser_util_public.int_($1, proc_ctx, $2); };

/*******************************************************************************
*******************************************************************************/

name: NAME;

path: name { $$ = cmap_parser_util_public.name(proc_ctx, $1); }
| path '.' name { $$ = cmap_parser_util_public.path($1, $3); }
| creator;

cmap: path | process | new | function
| NULL_PTR { $$ = NULL; };

/*******************************************************************************
*******************************************************************************/

args: { $$ = cmap_parser_util_public.args_empty(proc_ctx); }
| cmap { $$ = cmap_parser_util_public.args(proc_ctx, $1); }
| args ',' cmap { $$ = cmap_parser_util_public.args_push($1, $3); };

/*******************************************************************************
*******************************************************************************/

arg_names: { $$ = cmap_parser_util_public.args_empty(proc_ctx); }
| name { $$ = cmap_parser_util_public.arg_names(proc_ctx, $1); }
| arg_names ',' name
{
  $$ = cmap_parser_util_public.arg_names_push(proc_ctx, $1, $3);
};

/*******************************************************************************
*******************************************************************************/

arg_names_cmap: { $$ = cmap_parser_util_public.args_empty(proc_ctx); }
| name ':' cmap { $$ = cmap_parser_util_public.args_map(proc_ctx, $1, $3); }
| arg_names_cmap ',' name ':' cmap
{
  $$ = cmap_parser_util_public.args_map_push(proc_ctx, $1, $3, $5);
};

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
  $$ = cmap_parser_util_public.new(NULL, $2, $4, proc_ctx, $6);
}
| NEW path '.' name '(' args ')' aisle
{
  $$ = cmap_parser_util_public.new($2, $4, $6, proc_ctx, $8);
}
| NEW process_chain '.' name '(' args ')' aisle
{
  $$ = cmap_parser_util_public.new($2, $4, $6, proc_ctx, $8);
};

/*******************************************************************************
*******************************************************************************/

function: FUNCTION '(' arg_names ')' aisle '{' STRING '}'
{
  $$ = cmap_parser_util_public.function(proc_ctx, $5, $3, $7);
};

/*******************************************************************************
*******************************************************************************/

comparison: '(' comparison_int ')' { $$ = $2; };

comparison_int: cmap '<' cmap { $$ = cmap_cmp_public.lt($1, $3, proc_ctx); }
| cmap '>' cmap { $$ = cmap_cmp_public.gt($1, $3, proc_ctx); }
| cmap LE cmap { $$ = cmap_cmp_public.le($1, $3, proc_ctx); }
| cmap GE cmap { $$ = cmap_cmp_public.ge($1, $3, proc_ctx); }
| cmap EQUAL cmap { $$ = cmap_cmp_public.equal($1, $3, proc_ctx); }
| cmap DIFF cmap { $$ = cmap_cmp_public.diff($1, $3, proc_ctx); };

/*******************************************************************************
*******************************************************************************/

if: IF '(' STRING ')' '{' STRING '}' else
{
  $$ = cmap_parser_util_public.if_(proc_ctx, $8, $3, $6);
};

else: { $$ = NULL; }
| ELSE '{' STRING '}'
{
  $$ = cmap_parser_util_public.if_(proc_ctx, NULL, NULL, $3);
}
| ELSE if { $$ = $2; };

/*******************************************************************************
*******************************************************************************/

%%

static void cmap_parser_error(yyscan_t yyscanner, const char * msg)
{
  cmap_log_public.error("[%d:%d] %s", cmap_parser_get_lineno(yyscanner),
    cmap_parser_get_column(yyscanner), msg);
}
