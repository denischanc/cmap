%{

#include "cmap-scanner.h"
#include "cmap-parser.h"
#include "cmap-parser-arg.h"
#include "cmap-parser-block.h"
#include "cmap-parser-cmp.h"
#include "cmap-parser-creator.h"
#include "cmap-parser-part.h"
#include "cmap-parser-process.h"
#include "cmap-parser-var.h"
#include "cmap-part.h"
#include "cmap-build.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int cmap_parser_debug = 1;

static void cmap_parser_error(yyscan_t yyscanner, const char * msg);

%}

%define api.pure
%define parse.error detailed
%define parse.lac full
/*%define parse.trace*/

%param {yyscan_t yyscanner}

%union
{
  char * name;
}

%token FUNCTION_C STATIC_FUNCTION_C PROC IMPORT LOCAL NULL_PTR RETURN FUNCTION
%token IF ELSE NEW FOR WHILE SB2_O SB2_C LE GE EQUAL DIFF OR AND
%token ERROR
%token<name> STRING C_IMPL C_IMPL_ROOT INCLUDE NAME INT

%type<name> names creator creator_no_bracket cmap cmap_no_bracket
%type<name> args arg_names_cmap
%type<name> process_ret import_ret function
%type<name> comparison comparison_no_params comparison_deep comparison_simple
%type<name> for_iter

%%

/*******************************************************************************
*******************************************************************************/

start: include parts_or_instructions;

include: | INCLUDE { cmap_parser_part_public.include_($1); };

parts_or_instructions: parts
| instructions { cmap_parser_part_public.instructions_root(); };

parts: C_IMPL_ROOT { cmap_parser_part_public.c_impl_root($1); }
| function_c
| parts C_IMPL_ROOT { cmap_parser_part_public.c_impl_root($2); }
| parts function_c;

/*******************************************************************************
*******************************************************************************/

function_c: FUNCTION_C '(' NAME ')' '{' instructions '}'
  { cmap_parser_part_public.function_c($3, (1 == 0)); }
| STATIC_FUNCTION_C '(' NAME ')' '{' instructions '}'
  { cmap_parser_part_public.function_c($3, (1 == 1)); };

/*******************************************************************************
*******************************************************************************/

instructions: { cmap_part_public.push_instructions(); }
| instructions instruction ';'
| instructions C_IMPL { cmap_parser_part_public.c_impl($2); }
| instructions { cmap_parser_block_public.init_if(); } if
| instructions for
| instructions while;

/*******************************************************************************
*******************************************************************************/

names: NAME
| names '.' NAME { $$ = cmap_parser_arg_public.names($1, $3); };

instruction: LOCAL NAME '=' cmap { cmap_parser_var_public.set_local($2, $4); }
| NAME '=' cmap { cmap_parser_var_public.set_global($1, $3); }
| cmap '.' NAME '=' cmap { cmap_parser_var_public.set_path($1, $3, $5); }
| cmap SB2_O INT SB2_C '=' cmap
  { cmap_parser_var_public.set_sb_int($1, $3, $6); }
| cmap SB2_O STRING SB2_C '=' cmap
  { cmap_parser_var_public.set_sb_string($1, $3, $6); }
| cmap '[' cmap ']' '=' cmap { cmap_parser_var_public.set_sb_map($1, $3, $6); }
| process_no_ret
| RETURN cmap { cmap_parser_process_public.return_($2); }
| RETURN { cmap_parser_process_public.return_(NULL); }
| PROC '(' names ')' { cmap_parser_process_public.process_c($3, (1 == 0)); }
| import_no_ret;

/*******************************************************************************
*******************************************************************************/

creator: creator_no_bracket
| function
| NEW cmap_no_bracket '(' args ')'
  { $$ = cmap_parser_creator_public.new($2, $4); };

creator_no_bracket: STRING { $$ = cmap_parser_creator_public.string($1); }
| INT { $$ = cmap_parser_creator_public.int_($1); }
| '[' args ']' { $$ = cmap_parser_creator_public.list_args($2); }
| '{' arg_names_cmap '}' { $$ = cmap_parser_creator_public.map_args($2); };

/*******************************************************************************
*******************************************************************************/

cmap: creator
| NULL_PTR { $$ = strdup("NULL"); }
| process_ret
| NAME { $$ = cmap_parser_var_public.name($1); }
| cmap '.' NAME { $$ = cmap_parser_var_public.path($1, $3); }
| cmap SB2_O INT SB2_C { $$ = cmap_parser_var_public.sb_int($1, $3); }
| cmap SB2_O STRING SB2_C { $$ = cmap_parser_var_public.sb_string($1, $3); }
| cmap '[' cmap ']' { $$ = cmap_parser_var_public.sb_map($1, $3); }
| PROC '(' names ')'
  { $$ = cmap_parser_process_public.process_c($3, (1 == 1)); }
| import_ret;

cmap_no_bracket: creator_no_bracket
| NAME { $$ = cmap_parser_var_public.name($1); }
| cmap_no_bracket '.' NAME { $$ = cmap_parser_var_public.path($1, $3); }
| cmap_no_bracket SB2_O INT SB2_C
  { $$ = cmap_parser_var_public.sb_int($1, $3); }
| cmap_no_bracket SB2_O STRING SB2_C
  { $$ = cmap_parser_var_public.sb_string($1, $3); }
| cmap_no_bracket '[' cmap ']' { $$ = cmap_parser_var_public.sb_map($1, $3); }
| PROC '(' names ')'
  { $$ = cmap_parser_process_public.process_c($3, (1 == 1)); }
| import_ret;

/*******************************************************************************
*******************************************************************************/

args: { $$ = NULL; }
| cmap { $$ = cmap_parser_arg_public.args($1); }
| args ',' cmap { $$ = cmap_parser_arg_public.args_push($1, $3); };

/*******************************************************************************
*******************************************************************************/

arg_names:
| NAME { cmap_parser_arg_public.arg_name($1); }
| arg_names ',' NAME { cmap_parser_arg_public.arg_name($3); };

/*******************************************************************************
*******************************************************************************/

arg_names_cmap: { $$ = NULL; }
| names ':' cmap { $$ = cmap_parser_arg_public.args_map($1, $3); }
| arg_names_cmap ',' names ':' cmap
  { $$ = cmap_parser_arg_public.args_map_push($1, $3, $5); };

/*******************************************************************************
*******************************************************************************/

process_no_ret: NAME '(' args ')'
  { cmap_parser_process_public.process(NULL, $1, $3, (1 == 0)); }
| cmap '.' NAME '(' args ')'
  { cmap_parser_process_public.process($1, $3, $5, (1 == 0)); }
| function '(' args ')'
  { cmap_parser_process_public.process_fn($1, $3, (1 == 0)); };

process_ret: NAME '(' args ')'
  { $$ = cmap_parser_process_public.process(NULL, $1, $3, (1 == 1)); }
| cmap '.' NAME '(' args ')'
  { $$ = cmap_parser_process_public.process($1, $3, $5, (1 == 1)); }
| function '(' args ')'
  { $$ = cmap_parser_process_public.process_fn($1, $3, (1 == 1)); };

/*******************************************************************************
*******************************************************************************/

function: FUNCTION '(' arg_names ')'
  '{'
    { cmap_part_public.nature_ctx_fn(); } instructions
  '}' { $$ = cmap_parser_part_public.function(NULL); }
| FUNCTION '(' arg_names ')' '(' names ')'
  { $$ = cmap_parser_part_public.function($6); };

/*******************************************************************************
*******************************************************************************/

if: IF '(' comparison ')' '{' instructions '}'
  { cmap_parser_block_public.if_($3); } else;

else: { cmap_parser_block_public.else_empty(); }
| ELSE { cmap_parser_block_public.else_if(); } if
| ELSE '{' instructions '}' { cmap_parser_block_public.else_(); };

/*******************************************************************************
*******************************************************************************/

comparison: comparison_deep;

comparison_no_params:
  { cmap_part_public.ctx.rst_cmp_params(); }
  comparison_deep
  { cmap_part_public.ctx.set_cmp_params(); $$ = $2; };

comparison_deep:
  {
    cmap_part_public.nature_ctx_cmp();
    cmap_part_public.push_instructions();
  } comparison_simple { $$ = $2; }
| '(' comparison_deep ')' OR '(' comparison_deep ')'
  { $$ = cmap_parser_cmp_public.or($2, $6); }
| '(' comparison_deep ')' AND '(' comparison_deep ')'
  { $$ = cmap_parser_cmp_public.and($2, $6); };

comparison_simple: cmap { $$ = cmap_parser_cmp_public.cmp_unique($1); }
| cmap '<' cmap { $$ = cmap_parser_cmp_public.lt($1, $3); }
| cmap '>' cmap { $$ = cmap_parser_cmp_public.gt($1, $3); }
| cmap LE cmap { $$ = cmap_parser_cmp_public.le($1, $3); }
| cmap GE cmap { $$ = cmap_parser_cmp_public.ge($1, $3); }
| cmap EQUAL cmap { $$ = cmap_parser_cmp_public.equal($1, $3); }
| cmap DIFF cmap { $$ = cmap_parser_cmp_public.diff($1, $3); };

/*******************************************************************************
*******************************************************************************/

instructions_for:
| instruction
| instructions_for ',' instruction;

for_iter:
  {
    cmap_part_public.nature_ctx_iter();
    cmap_part_public.push_instructions();
  }
  instructions_for { $$ = cmap_parser_part_public.for_iter(); };

for: FOR '(' instructions_for ';' comparison_no_params ';' for_iter ')'
  '{'
    { cmap_part_public.nature_ctx_loop(); } instructions
  '}' { cmap_parser_block_public.for_($5, $7); };

/*******************************************************************************
*******************************************************************************/

while: WHILE '(' comparison_no_params ')'
  '{'
    { cmap_part_public.nature_ctx_loop(); } instructions
  '}' { cmap_parser_block_public.while_($3); };

/*******************************************************************************
*******************************************************************************/

import_no_ret: IMPORT '(' STRING ')'
  { if(!cmap_parser_process_public.import(NULL, $3, NULL)) YYABORT; }
| IMPORT '(' STRING ',' NAME ')'
  { if(!cmap_parser_process_public.import(NULL, $3, $5)) YYABORT; };

import_ret: IMPORT '(' STRING ')'
  { if(!cmap_parser_process_public.import(&$$, $3, NULL)) YYABORT; }
| IMPORT '(' STRING ',' NAME ')'
  { if(!cmap_parser_process_public.import(&$$, $3, $5)) YYABORT; };

/*******************************************************************************
*******************************************************************************/

%%

static void cmap_parser_error(yyscan_t yyscanner, const char * msg)
{
  fprintf(stderr, "[%s:%d:%d] %s\n", cmap_build_public.get_parse_path(),
    cmap_parser_get_lineno(yyscanner), cmap_parser_get_column(yyscanner), msg);
}
