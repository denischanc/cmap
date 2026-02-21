#ifndef __CMAP_PARSER_PART_H__
#define __CMAP_PARSER_PART_H__

void cmap_parser_part_append_instructions(const char * txt);

void cmap_parser_part_include(char * includes);
void cmap_parser_part_function_c(char * name, char is_static);
void cmap_parser_part_instructions_root();

char * cmap_parser_part_function(char * fn_name);

char * cmap_parser_part_function_cmp();

void cmap_parser_part_c_impl(char * impl);
void cmap_parser_part_c_impl_root(char * impl);

char * cmap_parser_part_for_iter();

#endif
