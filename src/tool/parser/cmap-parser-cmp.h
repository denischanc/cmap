#ifndef __CMAP_PARSER_CMP_H__
#define __CMAP_PARSER_CMP_H__

#include "cmap-parser-cmp-define.h"

typedef struct
{
  CMAP_PARSER_CMP_LOOP(CMAP_PARSER_CMP_DECL)
  char * (*cmp_unique)(char * map);
  char * (*cmp_unique_not)(char * map);

  char * (*init)(char * cmp_call);
  char * (*or_simple)(char * cmp_call_l, char * cmp_call_r);
  char * (*or)(char * cmp_call_l, char * cmp_call_r);
  char * (*and_simple)(char * cmp_call_l, char * cmp_call_r);
  char * (*and)(char * cmp_call_l, char * cmp_call_r);
} CMAP_PARSER_CMP_PUBLIC;

extern const CMAP_PARSER_CMP_PUBLIC cmap_parser_cmp_public;

#endif
