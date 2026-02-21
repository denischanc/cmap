#ifndef __CMAP_PART_H__
#define __CMAP_PART_H__

#include "cmap-part-define.h"
#include "cmap-part-ctx.h"
#include "cmap-part-var.h"
#include "cmap-strings.h"

const char * cmap_part_uid();

void cmap_part_set_cmp_params();
void cmap_part_rst_cmp_params();

void cmap_part_fn_arg_name(char * name);
void cmap_part_delete_fn_arg_names();

char cmap_part_is_definitions_n_set();
char cmap_part_is_global_env_n_set();

void cmap_part_set_return();
char cmap_part_is_return();

CMAP_STRINGS * cmap_part_get_params();

char cmap_part_return_fn();

CMAP_STRINGS * cmap_part_get_vars_def();

CMAP_PART_CTX * cmap_part_bup_ctx();
void cmap_part_restore_ctx(CMAP_PART_CTX * ctx);
void cmap_part_clean_ctx();

void cmap_part_loc(const char * name, const char * next_name);
char cmap_part_no_loc(const char * map, const char * name,
  const char * next_name);

CMAP_PART_VAR_RET cmap_part_get_map(const char * map, const char * name,
  char * next_name);

void cmap_part_clean_after_proc();

void cmap_part_clean();

CMAP_PART_LOOP(CMAP_PART_DECL)

CMAP_PART_CTX_NATURE_LOOP(CMAP_PART_NATURE_DECL)

void cmap_part_push_instructions();
char ** cmap_part_instructions();
void cmap_part_add_instruction(const char * instruction);
void cmap_part_add_variable(const char * variable);
void cmap_part_add_lf();
void cmap_part_prepend_instruction(const char * instruction);
char * cmap_part_pop_instructions();

void cmap_part_add_include(const char * name, char is_relative);

#endif
