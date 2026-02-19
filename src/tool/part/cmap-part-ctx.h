#ifndef __CMAP_PART_CTX_H__
#define __CMAP_PART_CTX_H__

#include "cmap-part-ctx-define.h"
#include "cmap-strings.h"
#include "cmap-part-kv.h"

typedef struct CMAP_PART_CTX CMAP_PART_CTX;

const char * cmap_part_ctx_uid();

CMAP_PART_CTX_NATURE_LOOP(CMAP_PART_CTX_NATURE_DECL)

CMAP_PART_CTX_FEATURE_LOOP(CMAP_PART_CTX_FEATURE_DECL)

void cmap_part_ctx_push();
char * cmap_part_ctx_pop();

CMAP_PART_CTX * cmap_part_ctx_c();
CMAP_PART_CTX * cmap_part_ctx_fn_c();
CMAP_PART_CTX * cmap_part_ctx_cmap();
CMAP_PART_CTX * cmap_part_ctx_cmap_prev(CMAP_PART_CTX * ctx);
CMAP_PART_CTX * cmap_part_ctx_c_prev(CMAP_PART_CTX * ctx);
CMAP_PART_CTX * cmap_part_ctx_block_next(CMAP_PART_CTX * ctx);
CMAP_PART_CTX * cmap_part_ctx_last_block(CMAP_PART_CTX * ctx);
CMAP_PART_CTX * cmap_part_ctx_block_prev(CMAP_PART_CTX * ctx);

char ** cmap_part_ctx_instructions(CMAP_PART_CTX * ctx);
const char * cmap_part_ctx_prefix(CMAP_PART_CTX * ctx);
void cmap_part_ctx_set_cmp_params();
void cmap_part_ctx_rst_cmp_params();
CMAP_STRINGS ** cmap_part_ctx_fn_arg_names(CMAP_PART_CTX * ctx);
CMAP_STRINGS ** cmap_part_ctx_affecteds(CMAP_PART_CTX * ctx);

char ** cmap_part_ctx_variables(CMAP_PART_CTX * ctx);
CMAP_PART_KV ** cmap_part_ctx_name2map(CMAP_PART_CTX * ctx);

char cmap_part_ctx_is_definitions_n_set();
char cmap_part_ctx_is_global_env_n_set();
void cmap_part_ctx_set_return();
char cmap_part_ctx_is_return();
CMAP_STRINGS ** cmap_part_ctx_params(CMAP_PART_CTX * ctx);

char cmap_part_ctx_return_fn(CMAP_PART_CTX * ctx);
CMAP_STRINGS ** cmap_part_ctx_vars_loc(CMAP_PART_CTX * ctx);
CMAP_STRINGS ** cmap_part_ctx_vars_def(CMAP_PART_CTX * ctx);

CMAP_STRINGS * cmap_part_ctx_prev_block_fn_arg_names(CMAP_PART_CTX * ctx);

CMAP_PART_CTX * cmap_part_ctx_split(CMAP_PART_CTX * ctx);
void cmap_part_ctx_join(CMAP_PART_CTX * ctx);
CMAP_PART_CTX * cmap_part_ctx_bup();
void cmap_part_ctx_restore(CMAP_PART_CTX * ctx);
void cmap_part_ctx_clean();

#endif
