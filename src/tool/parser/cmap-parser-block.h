#ifndef __CMAP_PARSER_BLOCK_H__
#define __CMAP_PARSER_BLOCK_H__

void cmap_parser_block_init_if();
void cmap_parser_block_if(char * cmp_call);
void cmap_parser_block_else_empty();
void cmap_parser_block_else_if();
void cmap_parser_block_else();

void cmap_parser_block_for(char * cmp_call, char * iter_call);
void cmap_parser_block_while(char * cmp_call);

void cmap_parser_block_clean();

#endif
