#ifndef __CMAP_PARSER_PARAMS_H__
#define __CMAP_PARSER_PARAMS_H__

typedef struct
{
  char * decl;
  char * impl;
} CMAP_PARSER_PARAMS_RET;

void cmap_parser_params_clone();
CMAP_PARSER_PARAMS_RET cmap_parser_params_get();
void cmap_parser_params_delete(CMAP_PARSER_PARAMS_RET ret);

#endif
