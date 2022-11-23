%{

/* TODO : kernel mem usage for scanner, parser */

#include "cmap-scanner.h"
#include "cmap-parser-util.h"

static void string_scanner(yyscan_t scanner, YYSTYPE * yylval_p);
static void scanner_append(YYSTYPE * yylval_p, const char * txt);

%}

%option noyywrap noinput nounput yylineno warn bison-bridge reentrant

%x COMMENT_MULTI_S COMMENT_SIMPLE_S STRING_S

NAME_D [[:alpha:]_$][[:alnum:]_$\-]*
SPACE_D [[:space:]]+

%%

"/*" { BEGIN(COMMENT_MULTI_S); }
<COMMENT_MULTI_S>"*/" { BEGIN(INITIAL); }
<COMMENT_MULTI_S>"\n" {}
<COMMENT_MULTI_S>. {}

"//" { BEGIN(COMMENT_SIMPLE_S); }
<COMMENT_SIMPLE_S>"\n" { BEGIN(INITIAL); }
<COMMENT_SIMPLE_S>. {}

"\"" { BEGIN(STRING_S); string_scanner(yyscanner, yylval); }
<STRING_S>"\"" { BEGIN(INITIAL); return STRING; }
<STRING_S>"\\t" { scanner_append(yylval, "\t"); }
<STRING_S>"\\n" { scanner_append(yylval, "\n"); }
<STRING_S>"\\". { scanner_append(yylval, yytext + 1); }
<STRING_S>. { scanner_append(yylval, yytext); }

"local" {
  /* TODO : free ptr */
  yylval -> name = strdup(yytext);
  yylval -> name[yyleng] = 0;
  return LOCAL;
}

{NAME_D} {
  /* TODO : free ptr */
  yylval -> name = strdup(yytext);
  yylval -> name[yyleng] = 0;
  return NAME;
}

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
"<" { return '<'; }
">" { return '>'; }

{SPACE_D} {}
"\n" {}
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
