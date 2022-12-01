#ifndef __CMAP_SCANNER_H__
#define __CMAP_SCANNER_H__

typedef void * yyscan_t;

#include <cmap/core.h>
#include "cmap-proc-ctx.h"
#include "cmap-parser.h"
#include <stdio.h>

#define YY_EXTRA_TYPE CMAP_PROC_CTX *

int cmap_parser_lex(YYSTYPE * yylval_p, yyscan_t scanner);

int cmap_parser_lex_init_extra(YY_EXTRA_TYPE user_defined, yyscan_t * scanner);
int cmap_parser_lex_destroy(yyscan_t scanner);

void cmap_parser_set_in(FILE * in, yyscan_t scanner);
FILE * cmap_parser_get_in(yyscan_t scanner);

int cmap_parser_get_lineno(yyscan_t scanner);
int cmap_parser_get_column(yyscan_t scanner);

#endif
