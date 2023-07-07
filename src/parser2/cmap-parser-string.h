#ifndef __CMAP_PARSER_STRING_H__
#define __CMAP_PARSER_STRING_H__

#include <stdio.h>

typedef struct CMAP_PARSER_STRING CMAP_PARSER_STRING;

CMAP_PARSER_STRING * cmap_parser_string_create(const char * txt);
CMAP_PARSER_STRING * cmap_parser_string_create_args(const char * txt, ...);

void cmap_parser_string_append(CMAP_PARSER_STRING * string,
  const char * txt);
void cmap_parser_string_append_args(CMAP_PARSER_STRING * string,
  const char * txt, ...);
void cmap_parser_string_append_string(CMAP_PARSER_STRING * dst,
  CMAP_PARSER_STRING * src);

void cmap_parser_string_prepend(CMAP_PARSER_STRING * string,
  const char * txt);
void cmap_parser_string_prepend_args(CMAP_PARSER_STRING * string,
  const char * txt, ...);

void cmap_parser_string_fprintf(FILE * f, CMAP_PARSER_STRING * string);

void cmap_parser_string_clear(CMAP_PARSER_STRING * string);

void cmap_parser_string_delete(CMAP_PARSER_STRING * string);

char * cmap_parser_string_dupndel(CMAP_PARSER_STRING * string);

#endif
