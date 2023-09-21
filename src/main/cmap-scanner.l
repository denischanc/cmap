%{

#include "cmap-scanner.h"
#include "cmap-string.h"

#define YY_USER_ACTION yycolumn += yyleng;

static void string_start(const char * txt);
static void string_append(const char * txt);
static void string_end(YYSTYPE * yylval_p, const char * txt);

static void set_name(YYSTYPE * yylval_p, const char * name);

%}

%option noyywrap noinput nounput yylineno warn bison-bridge reentrant

%x COMMENT_MULTI_S COMMENT_SIMPLE_S STRING_S C_IMPL_S

NAME_D [[:alpha:]_$][[:alnum:]_$\-]*
INT_D [[:digit:]]+
SPACE_D [[:space:]]+

%%

"/*" { BEGIN(COMMENT_MULTI_S); }
<COMMENT_MULTI_S>{
  "*/" { BEGIN(INITIAL); }
  .
  "\n"
}

"//" { BEGIN(COMMENT_SIMPLE_S); }
<COMMENT_SIMPLE_S>{
  "\n" { BEGIN(INITIAL); }
  .
}

"\"" { BEGIN(STRING_S); string_start("\""); }
<STRING_S>{
  "\"" { BEGIN(INITIAL); string_end(yylval, "\""); return STRING; }
  . { string_append(yytext); }
  "\n" { string_append(yytext); }
}

[[:space:]]*"[C["[[:space:]]* { BEGIN(C_IMPL_S); string_start(""); }
<C_IMPL_S>{
  [[:space:]]*"]]" { BEGIN(INITIAL); string_end(yylval, ""); return C_IMPL; }
  . { string_append(yytext); }
  "\n" { string_append(yytext); }
}

"#include" { return INCLUDE; }
"@FUNCTION" { return FUNCTION_C; }
"@STATIC_FUNCTION" { return STATIC_FUNCTION_C; }
"LOCAL" { return LOCAL; }
"null" { return NULL_PTR; }
"return" { return RETURN; }
"function" { return FUNCTION; }
"@PROC" { return PROC; }
"if" { return IF; }
"else" { return ELSE; }

{NAME_D} { set_name(yylval, yytext); return NAME; }
{INT_D} { set_name(yylval, yytext); return INT; }

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

static char * string;

static void string_start(const char * txt)
{
  string = strdup(txt);
}

static void string_append(const char * txt)
{
  cmap_parser_string_public.append(&string, txt);
}

static void string_end(YYSTYPE * yylval_p, const char * txt)
{
  cmap_parser_string_public.append(&string, txt);
  yylval_p -> name = string;
}

static void set_name(YYSTYPE * yylval_p, const char * name)
{
  yylval_p -> name = strdup(name);
}
