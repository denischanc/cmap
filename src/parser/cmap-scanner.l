%{

/* TODO : kernel mem usage for scanner, parser */

#include "cmap-scanner.h"
#include "cmap-parser-util.h"
#include "cmap-kernel.h"
#include "cmap-util.h"

#define YY_USER_ACTION yycolumn += yyleng;

static void string_scanner(YYSTYPE * yylval_p, CMAP_PROC_CTX * proc_ctx);
static void string_scanner_with_txt(YYSTYPE * yylval_p,
  CMAP_PROC_CTX * proc_ctx, const char * txt);
static void scanner_append(YYSTYPE * yylval_p, const char * txt);
static void name_scanner(YYSTYPE * yylval_p, const char * txt);

%}

%option noyywrap noinput nounput yylineno warn bison-bridge reentrant

%x COMMENT_MULTI_S COMMENT_SIMPLE_S STRING_S IMPL_S CMP_S
%s BLOCK_S IF_S

NAME_D [[:alpha:]_$][[:alnum:]_$\-]*
INT_D [[:digit:]]+
SPACE_D [[:space:]]+

%%

"/*" { BEGIN(COMMENT_MULTI_S); }
<COMMENT_MULTI_S>{
  "*/" { BEGIN(INITIAL); }
  .
}

"//" { BEGIN(COMMENT_SIMPLE_S); }
<COMMENT_SIMPLE_S>{
  "\n" { BEGIN(INITIAL); }
  .
}

"\"" { BEGIN(STRING_S); string_scanner(yylval, yyextra); }
<STRING_S>{
  "\"" { BEGIN(INITIAL); return STRING; }
  "\\t" { scanner_append(yylval, "\t"); }
  "\\n" { scanner_append(yylval, "\n"); }
  "\\". { scanner_append(yylval, yytext + 1); }
  . { scanner_append(yylval, yytext); }
}

<BLOCK_S>"{" { BEGIN(IMPL_S); return '{'; }
<IMPL_S>[^}]* {
  BEGIN(INITIAL);
  string_scanner_with_txt(yylval, yyextra, yytext);
  return STRING;
}

<IF_S>"(" { BEGIN(CMP_S); return '('; }
<CMP_S>[^)]* {
  BEGIN(BLOCK_S);
  string_scanner_with_txt(yylval, yyextra, yytext);
  return STRING;
}

"local" { name_scanner(yylval, yytext); return LOCAL; }
"new" { name_scanner(yylval, yytext); return NEW; }
"return" { name_scanner(yylval, yytext); return RETURN; }

"null" { return NULL_PTR; }
"function" { BEGIN(BLOCK_S); return FUNCTION; }
"if" { BEGIN(IF_S); return IF; }
"else" { BEGIN(BLOCK_S); return ELSE; }

{NAME_D} { name_scanner(yylval, yytext); return NAME; }
{INT_D} { yylval -> int_ = atol(yytext); return INT; }

"." { return '.'; }
"," { return ','; }
";" { return ';'; }
"=" { return '='; }
"(" { return '('; }
")" { return ')'; }
"[" { return '['; }
"]" { return ']'; }
"{" { return '{'; }
"}" { return '}'; }
"/" { return '/'; }
":" { return ':'; }

"<" { return '<'; }
">" { return '>'; }
"<=" { return LE; }
">=" { return GE; }
"==" { return EQUAL; }
"!=" { return DIFF; }

{SPACE_D}

. { return ERROR; }

%%

static void string_scanner(YYSTYPE * yylval_p, CMAP_PROC_CTX * proc_ctx)
{
  yylval_p -> string = cmap_parser_util_public.string_scanner(proc_ctx);
}

static void string_scanner_with_txt(YYSTYPE * yylval_p,
  CMAP_PROC_CTX * proc_ctx, const char * txt)
{
  string_scanner(yylval_p, proc_ctx);
  scanner_append(yylval_p, txt);
}

static void scanner_append(YYSTYPE * yylval_p, const char * txt)
{
  cmap_parser_util_public.scanner_append(yylval_p -> string, txt);
}

static void name_scanner(YYSTYPE * yylval_p, const char * txt)
{
  yylval_p -> name = cmap_util_public.strdup(txt);
}
