%{

#include "cmap-scanner.h"
#include "cmap-parser.h"
#include "cmap-parser-util.h"
#include "cmap-part.h"
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

%token FUNCTION_C STATIC_FUNCTION_C LOCAL NULL_PTR RETURN FUNCTION PROC
%token IF ELSE LE GE EQUAL DIFF NEW SB2_O SB2_C H2 CMAP_IMPL
%token ERROR
%token<name> STRING C_IMPL NAME INT

%type<name> cmap aisle arg_names_cmap creator args process function arg_names
%type<name> comparison comparison_ cmap_not_creator names

%%

/*******************************************************************************
*******************************************************************************/

start: parts
| CMAP_IMPL instructions { cmap_parser_util_public.cmap_impl(); };

parts:
| parts C_IMPL { cmap_parser_util_public.c_impl_root($2); }
| parts function_c;

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

instructions: { cmap_part_public.push_instructions(); }
| instructions instruction ';'
| instructions C_IMPL { cmap_parser_util_public.c_impl($2); }
| instructions if;

/*******************************************************************************
*******************************************************************************/

names: NAME
| names '.' NAME { $$ = cmap_parser_util_public.names($1, $3); };

instruction: LOCAL NAME '=' cmap { cmap_parser_util_public.set_local($2, $4); }
| NAME '=' cmap { cmap_parser_util_public.set_global($1, $3); }
| cmap '.' NAME '=' cmap { cmap_parser_util_public.set_path($1, $3, $5); }
| cmap SB2_O INT SB2_C '=' cmap
{
  cmap_parser_util_public.set_sb_int($1, $3, $6);
}
| cmap SB2_O STRING SB2_C '=' cmap
{
  cmap_parser_util_public.set_sb_string($1, $3, $6);
}
| cmap '[' cmap ']' '=' cmap
{
  cmap_parser_util_public.set_sb_map($1, $3, $6);
}
| process { free($1); }
| RETURN cmap { cmap_parser_util_public.return_($2); }
| RETURN { cmap_parser_util_public.return_(NULL); }
| PROC '(' names ')' { cmap_parser_util_public.process_c($3, (1 == 0)); };

/*******************************************************************************
*******************************************************************************/

aisle: { $$ = NULL; }
| '#' names '#' { $$ = cmap_parser_util_public.aisle_names($2); }
| H2 cmap_not_creator H2 { $$ = cmap_parser_util_public.aisle_map($2); };

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
| INT aisle { $$ = cmap_parser_util_public.int_($1, $2); }
| function
| NEW cmap '{' args '}' aisle { $$ = cmap_parser_util_public.new($2, $4, $6); };

/*******************************************************************************
*******************************************************************************/

cmap: creator | cmap_not_creator;

cmap_not_creator: NULL_PTR { $$ = strdup("NULL"); } | process
| NAME { $$ = cmap_parser_util_public.name($1); }
| cmap '.' NAME { $$ = cmap_parser_util_public.path($1, $3); }
| cmap SB2_O INT SB2_C { $$ = cmap_parser_util_public.sb_int($1, $3); }
| cmap SB2_O STRING SB2_C { $$ = cmap_parser_util_public.sb_string($1, $3); }
| cmap '[' cmap ']' { $$ = cmap_parser_util_public.sb_map($1, $3); }
| PROC '(' names ')' { $$ = cmap_parser_util_public.process_c($3, (1 == 1)); };

/*******************************************************************************
*******************************************************************************/

args: { $$ = NULL; }
| cmap
| args ',' cmap { $$ = cmap_parser_util_public.args_push($1, $3); };

/*******************************************************************************
*******************************************************************************/

arg_names: { $$ = NULL; }
| NAME { $$ = cmap_parser_util_public.arg_names($1); }
| arg_names ',' NAME
{
  $$ = cmap_parser_util_public.arg_names_push($1, $3);
};

/*******************************************************************************
*******************************************************************************/

arg_names_cmap: { $$ = NULL; }
| names ':' cmap { $$ = cmap_parser_util_public.args_map($1, $3); }
| arg_names_cmap ',' names ':' cmap
{
  $$ = cmap_parser_util_public.args_map_push($1, $3, $5);
};

/*******************************************************************************
*******************************************************************************/

process: NAME '(' args ')'
{
  $$ = cmap_parser_util_public.process(NULL, $1, $3);
}
| cmap '.' NAME '(' args ')'
{
  $$ = cmap_parser_util_public.process($1, $3, $5);
}
| function '(' args ')' { $$ = cmap_parser_util_public.process_fn($1, $3); };

/*******************************************************************************
*******************************************************************************/

function: FUNCTION '(' arg_names ')' aisle '{'
{
  cmap_part_public.new_ctx(CMAP_PART_CTX_NATURE_FN);
} instructions '}'
{
  $$ = cmap_parser_util_public.function($3, $5, NULL);
}
| FUNCTION '(' arg_names ')' aisle '(' names ')'
{
  $$ = cmap_parser_util_public.function($3, $5, $7);
};

/*******************************************************************************
*******************************************************************************/

if: IF '(' comparison ')' '{' instructions '}'
{
  cmap_parser_util_public.if_($3);
} else;

else: { cmap_parser_util_public.else_empty(); }
| ELSE { cmap_parser_util_public.else_if_start(); }
  if { cmap_parser_util_public.else_if_stop(); }
| ELSE '{' instructions '}' { cmap_parser_util_public.else_(); };

/*******************************************************************************
*******************************************************************************/

comparison:
{
  cmap_part_public.new_ctx(CMAP_PART_CTX_NATURE_DFT);
  cmap_part_public.push_instructions();
} comparison_ { $$ = $2; };

comparison_: cmap { $$ = cmap_parser_util_public.cmp_unique($1); }
| cmap '<' cmap { $$ = cmap_parser_util_public.lt($1, $3); }
| cmap '>' cmap { $$ = cmap_parser_util_public.gt($1, $3); }
| cmap LE cmap { $$ = cmap_parser_util_public.le($1, $3); }
| cmap GE cmap { $$ = cmap_parser_util_public.ge($1, $3); }
| cmap EQUAL cmap { $$ = cmap_parser_util_public.equal($1, $3); }
| cmap DIFF cmap { $$ = cmap_parser_util_public.diff($1, $3); };

/*******************************************************************************
*******************************************************************************/

%%

static void cmap_parser_error(yyscan_t yyscanner, const char * msg)
{
  fprintf(stderr, "[%d:%d] %s\n", cmap_parser_get_lineno(yyscanner),
    cmap_parser_get_column(yyscanner), msg);
}
