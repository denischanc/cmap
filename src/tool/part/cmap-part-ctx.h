#ifndef __CMAP_PART_CTX_H__
#define __CMAP_PART_CTX_H__

#include "cmap-part-ctx-define.h"
#include "cmap-strings.h"
#include "cmap-part-keys.h"
#include "cmap-part-kv.h"

typedef struct CMAP_PART_CTX CMAP_PART_CTX;

typedef struct
{
  CMAP_PART_CTX_NATURE_LOOP(CMAP_PART_CTX_NATURE_DECL)

  void (*cmp_params)();
  void (*cmp_no_params)();

  char (*is_params)(CMAP_PART_CTX * ctx);

  void (*push)();
  char * (*pop)();

  char ** (*instructions)(CMAP_PART_CTX * ctx);
  const char * (*prefix)();
  void (*else_)();
  char (*is_else)();
  CMAP_STRINGS ** (*fn_arg_names)(CMAP_PART_CTX * ctx);
  CMAP_PART_KEYS ** (*affecteds)(CMAP_PART_CTX * ctx);

  char (*is_definitions)();
  char (*is_global_env)();
  void (*return_)();
  char (*is_return)();
  void (*return_fn)();
  char (*is_return_fn)();
  CMAP_PART_KV ** (*name2map)(CMAP_PART_CTX * ctx);
  CMAP_STRINGS ** (*params)(CMAP_PART_CTX * ctx);

  CMAP_STRINGS ** (*vars_loc)(CMAP_PART_CTX * ctx);
  CMAP_STRINGS ** (*vars_def)(CMAP_PART_CTX * ctx);

  CMAP_PART_CTX * (*c)();
  CMAP_PART_CTX * (*cmap_prev)(CMAP_PART_CTX * ctx);
  CMAP_PART_CTX * (*c_prev)(CMAP_PART_CTX * ctx);
  CMAP_PART_CTX * (*block_next)(CMAP_PART_CTX * ctx);
  CMAP_PART_CTX * (*last_block)(CMAP_PART_CTX * ctx);

  void (*restore)(CMAP_PART_CTX * ctx);
  CMAP_PART_CTX * (*bup)(CMAP_PART_CTX * ctx);
} CMAP_PART_CTX_PUBLIC;

extern const CMAP_PART_CTX_PUBLIC cmap_part_ctx_public;

#endif
