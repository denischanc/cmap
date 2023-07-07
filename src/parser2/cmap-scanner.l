%{

#include "cmap-scanner.h"
#include "cmap-parser-string.h"

#define YY_USER_ACTION yycolumn += yyleng;

static void string_start();
static void string_append(const char * txt);
static void string_end(YYSTYPE * yylval_p);

static void set_name(YYSTYPE * yylval_p, const char * name);

%}

%option noyywrap noinput nounput yylineno warn bison-bridge reentrant

%x COMMENT_MULTI_S COMMENT_SIMPLE_S STRING_S

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

"\"" { BEGIN(STRING_S); string_start(); }
<STRING_S>{
  "\"" { BEGIN(INITIAL); string_end(yylval); return STRING; }
  . { string_append(yytext); }
}

"#include" { return INCLUDE; }
"@FUNCTION" { return FUNCTION_C; }
"local" { return LOCAL; }
"new" { return NEW; }
"return" { return RETURN; }
"null" { return NULL_PTR; }
"function" { return FUNCTION; }
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

"\n"

. { return ERROR; }

%%

static CMAP_PARSER_STRING * string_;

static void string_start()
{
  string_ = cmap_parser_string_create("\"");
}

static void string_append(const char * txt)
{
  cmap_parser_string_append(string_, txt);
}

static void string_end(YYSTYPE * yylval_p)
{
  cmap_parser_string_append(string_, "\"");
  yylval_p -> name = cmap_parser_string_dupndel(string_);
}

static void set_name(YYSTYPE * yylval_p, const char * name)
{
  yylval_p -> name = strdup(name);
}
