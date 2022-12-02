%{

/* TODO : kernel mem usage for scanner, parser */

#include "cmap-scanner.h"
#include "cmap-parser-util.h"
#include "cmap-kernel.h"

static void string_scanner(yyscan_t scanner, YYSTYPE * yylval_p);
static void scanner_append(YYSTYPE * yylval_p, const char * txt);
static void name_scanner(YYSTYPE * yylval_p, const char * txt);

%}

%option noyywrap noinput nounput yylineno warn bison-bridge reentrant

%x COMMENT_MULTI_S COMMENT_SIMPLE_S STRING_S IMPL_S
%s FUNCTION_S

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

"\"" { BEGIN(STRING_S); string_scanner(yyscanner, yylval); }
<STRING_S>{
  "\"" { BEGIN(INITIAL); return STRING; }
  "\\t" { scanner_append(yylval, "\t"); }
  "\\n" { scanner_append(yylval, "\n"); }
  "\\". { scanner_append(yylval, yytext + 1); }
  . { scanner_append(yylval, yytext); }
}

<FUNCTION_S>"{" { BEGIN(IMPL_S); string_scanner(yyscanner, yylval); }
<IMPL_S>{
  "}" { BEGIN(INITIAL); return STRING; }
  . { scanner_append(yylval, yytext); }
}

"local" { name_scanner(yylval, yytext); return LOCAL; }
"new" { name_scanner(yylval, yytext); return NEW; }
"return" { name_scanner(yylval, yytext); return RETURN; }
"null" { name_scanner(yylval, yytext); return NULL_PTR; }
"function" { BEGIN(FUNCTION_S); return FUNCTION; }

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
"\n"
. { return ERROR; }

%%

static void string_scanner(yyscan_t scanner, YYSTYPE * yylval_p)
{
  CMAP_PROC_CTX * proc_ctx = cmap_parser_get_extra(scanner);
  yylval_p -> string = cmap_parser_util_public.string_scanner(proc_ctx);
}

static void scanner_append(YYSTYPE * yylval_p, const char * txt)
{
  cmap_parser_util_public.scanner_append(yylval_p -> string, txt);
}

static void name_scanner(YYSTYPE * yylval_p, const char * txt)
{
  int size = (strlen(txt) + 1) * sizeof(char);
  yylval_p -> name = (char *)cmap_kernel_public.mem() -> alloc(size);
  memcpy(yylval_p -> name, txt, size);
}
