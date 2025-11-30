%{

#include "cmap-scanner.h"
#include "cmap-string.h"
#include "cmap-do-parse.h"
#include "cmap-file-util.h"

#define YY_USER_ACTION yycolumn += yyleng;

static void string_start(const char * txt);
static void string_append(const char * txt);
static void string_end(YYSTYPE * yylval_p, const char * txt);

static void set_name(YYSTYPE * yylval_p, const char * name);

static void set_dir(YYSTYPE * yylval_p);
static void set_file(YYSTYPE * yylval_p);

%}

%option noyywrap noinput nounput yylineno warn bison-bridge reentrant

%x COMMENT_MULTI_S COMMENT_SIMPLE_S STRING_S C_IMPL_S C_IMPL_ROOT_S INCLUDE_S

NAME_D [[:alpha:]_$][[:alnum:]_$@\-]*
INT_D \-?[[:digit:]]+
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
  "\\\\" { string_append(yytext); }
  "\\\"" { string_append(yytext); }
  "\"" { BEGIN(INITIAL); string_end(yylval, "\""); return STRING; }
  . { string_append(yytext); }
  "\n" { string_append(yytext); }
}

"@C"[[:space:]]*"{{"[[:space:]]* { BEGIN(C_IMPL_S); string_start(""); }
<C_IMPL_S>{
  [[:space:]]*"}}" { BEGIN(INITIAL); string_end(yylval, ""); return C_IMPL; }
  . { string_append(yytext); }
  "\n" { string_append(yytext); }
}

"@C_ROOT"[[:space:]]*"{{"[[:space:]]* { BEGIN(C_IMPL_ROOT_S); string_start("");
  }
<C_IMPL_ROOT_S>{
  [[:space:]]*"}}" { BEGIN(INITIAL); string_end(yylval, "");
    return C_IMPL_ROOT; }
  . { string_append(yytext); }
  "\n" { string_append(yytext); }
}

"@INCLUDE"[[:space:]]*"{"[[:space:]]* { BEGIN(INCLUDE_S); string_start(""); }
<INCLUDE_S>{
  [[:space:]]*"}" { BEGIN(INITIAL); string_end(yylval, ""); return INCLUDE; }
  . { string_append(yytext); }
  "\n" { string_append(yytext); }
}

"@FUNCTION" { return FUNCTION_C; }
"@STATIC_FUNCTION" { return STATIC_FUNCTION_C; }
"@PROC" { return PROC; }
"@IMPORT" { return IMPORT; }
"@DIR" { set_dir(yylval); return STRING; }
"@FILE" { set_file(yylval); return STRING; }

"local" { return LOCAL; }
"var" { return LOCAL; }
"null" { return NULL_PTR; }
"return" { return RETURN; }
"function" { return FUNCTION; }
"if" { return IF; }
"else" { return ELSE; }
"new" { return NEW; }
"for" { return FOR; }
"while" { return WHILE; }

{NAME_D} { set_name(yylval, yytext); return NAME; }
{INT_D} { set_name(yylval, yytext); return INT; }

"[[" { return SB2_O; }
"]]" { return SB2_C; }

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
":" { return ':'; }
"!" { return '!'; }

"<" { return '<'; }
">" { return '>'; }
"<=" { return LE; }
">=" { return GE; }
"==" { return EQUAL; }
"!=" { return DIFF; }
"||" { return OR; }
"&&" { return AND; }

"+" { return '+'; }
"-" { return '-'; }
"*" { return '*'; }
"/" { return '/'; }
"+=" { return ADD_SELF; }
"-=" { return SUB_SELF; }
"*=" { return MUL_SELF; }
"/=" { return DIV_SELF; }
"++" { return INC; }
"--" { return DEC; }

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
  cmap_string_public.append(&string, txt);
}

static void string_end(YYSTYPE * yylval_p, const char * txt)
{
  cmap_string_public.append(&string, txt);
  yylval_p -> name = string;
}

static void set_name(YYSTYPE * yylval_p, const char * name)
{
  yylval_p -> name = strdup(name);
}

static void set_dir(YYSTYPE * yylval_p)
{
  char * string = NULL,
    * dir = cmap_file_util_public.dirname(cmap_do_parse_public.path());
  cmap_string_public.append_args(&string, "\"%s\"", dir);
  free(dir);
  yylval_p -> name = string;
}

static void set_file(YYSTYPE * yylval_p)
{
  char * string = NULL;
  cmap_string_public.append_args(&string,
    "\"%s\"", cmap_do_parse_public.path());
  yylval_p -> name = string;
}
