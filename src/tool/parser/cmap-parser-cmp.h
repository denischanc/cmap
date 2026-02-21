#ifndef __CMAP_PARSER_CMP_H__
#define __CMAP_PARSER_CMP_H__

#include "cmap-parser-cmp-define.h"

CMAP_PARSER_CMP_LOOP(CMAP_PARSER_CMP_DECL)
char * cmap_parser_cmp_unique(char * map);
char * cmap_parser_cmp_unique_not(char * map);

char * cmap_parser_cmp_init(char * cmp_call);
char * cmap_parser_cmp_or_simple(char * cmp_call_l, char * cmp_call_r);
char * cmap_parser_cmp_or(char * cmp_call_l, char * cmp_call_r);
char * cmap_parser_cmp_and_simple(char * cmp_call_l, char * cmp_call_r);
char * cmap_parser_cmp_and(char * cmp_call_l, char * cmp_call_r);

#endif
