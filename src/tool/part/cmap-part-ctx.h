#ifndef __CMAP_PART_CTX_H__
#define __CMAP_PART_CTX_H__

#include "cmap-part-ctx-define.h"
#include "cmap-strings.h"
#include "cmap-part-kv.h"

typedef struct CMAP_PART_CTX CMAP_PART_CTX;

typedef struct
{
  const char * (*uid)();

  CMAP_PART_CTX_NATURE_LOOP(CMAP_PART_CTX_NATURE_DECL)

  char (*is_feature_params)(CMAP_PART_CTX * ctx);

  void (*push)();
  char * (*pop)();

  CMAP_PART_CTX * (*c)();
  CMAP_PART_CTX * (*fn_c)();
  CMAP_PART_CTX * (*cmap)();
  CMAP_PART_CTX * (*cmap_prev)(CMAP_PART_CTX * ctx);
  CMAP_PART_CTX * (*c_prev)(CMAP_PART_CTX * ctx);
  CMAP_PART_CTX * (*block_next)(CMAP_PART_CTX * ctx);
  CMAP_PART_CTX * (*last_block)(CMAP_PART_CTX * ctx);

  char ** (*instructions)(CMAP_PART_CTX * ctx);
  const char * (*prefix)(CMAP_PART_CTX * ctx);
  void (*set_else)();
  char (*is_else_n_rst)();
  void (*rst_cmp_params)();
  CMAP_STRINGS ** (*fn_arg_names)(CMAP_PART_CTX * ctx);
  CMAP_STRINGS ** (*affecteds)(CMAP_PART_CTX * ctx);

  char ** (*variables)(CMAP_PART_CTX * ctx);
  CMAP_PART_KV ** (*name2map)(CMAP_PART_CTX * ctx);

  char (*is_definitions_n_set)();
  char (*is_global_env_n_set)();
  void (*set_return)();
  char (*is_return)();
  char (*return_fn)(CMAP_PART_CTX * ctx);
  CMAP_STRINGS ** (*params)(CMAP_PART_CTX * ctx);

  CMAP_STRINGS ** (*vars_loc)(CMAP_PART_CTX * ctx);
  CMAP_STRINGS ** (*vars_def)(CMAP_PART_CTX * ctx);

  CMAP_STRINGS * (*prev_block_fn_arg_names)(CMAP_PART_CTX * ctx);

  void (*restore)(CMAP_PART_CTX * ctx);
  CMAP_PART_CTX * (*bup)(CMAP_PART_CTX * ctx);
} CMAP_PART_CTX_PUBLIC;

extern const CMAP_PART_CTX_PUBLIC cmap_part_ctx_public;

#endif
