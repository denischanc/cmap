#ifndef __CMAP_EXT_H__
#define __CMAP_EXT_H__

#include "cmap-kernel-type.h"
#include "cmap-proc-ctx-type.h"
#include "cmap-map-type.h"
#include "cmap-fn-type.h"
#include "cmap-string-type.h"
#include "cmap-int-type.h"
#include "cmap-double-type.h"
#include "cmap-ptr-type.h"
#include "cmap-env-type.h"
#include "cmap-define-ext.h"
#include <stdint.h>

void cmap_bootstrap(CMAP_KERNEL_CFG * cfg);
int cmap_main();
void cmap_exit(int ret);
void cmap_fatal();

CMAP_MAP * cmap_map(CMAP_PROC_CTX * proc_ctx, const char * aisle);
CMAP_LIST * cmap_list(int size_inc, CMAP_PROC_CTX * proc_ctx,
  const char * aisle);
CMAP_FN * cmap_fn(CMAP_FN_TPL process, CMAP_PROC_CTX * proc_ctx,
  const char * aisle);
CMAP_STRING * cmap_string(const char * val, int size_inc,
  CMAP_PROC_CTX * proc_ctx, const char * aisle);
CMAP_INT * cmap_int(int64_t val, CMAP_PROC_CTX * proc_ctx, const char * aisle);
CMAP_DOUBLE * cmap_double(double val, CMAP_PROC_CTX * proc_ctx,
  const char * aisle);
CMAP_PTR * cmap_ptr(int size, CMAP_PTR_DELETE delete_ptr,
  CMAP_PROC_CTX * proc_ctx, const char * aisle);

const char * cmap_nature(CMAP_MAP * map);
CMAP_MAP * cmap_delete(CMAP_MAP * map);
void cmap_set(CMAP_MAP * map, const char * key, CMAP_MAP * val);
CMAP_MAP * cmap_get(CMAP_MAP * map, const char * key);
char cmap_is_key(CMAP_MAP * map, const char * key);

void cmap_list_set(CMAP_LIST * list, int i, CMAP_MAP * val);
CMAP_MAP * cmap_list_get(CMAP_LIST * list, int i);

void cmap_int_set(CMAP_INT * i, int64_t val);
int64_t cmap_int_get(CMAP_INT * i);

void cmap_double_set(CMAP_DOUBLE * d, double val);
double cmap_double_get(CMAP_DOUBLE * d);

void * cmap_ptr_get(CMAP_PTR * ptr);
void ** cmap_ptr_ref(CMAP_PTR * ptr);

const char * cmap_string_val(CMAP_STRING * string);

CMAP_MAP * cmap_fn_require_definitions(CMAP_FN * fn, CMAP_PROC_CTX * proc_ctx);
void cmap_add_arg_names(CMAP_FN * fn, CMAP_PROC_CTX * proc_ctx, ...);

CMAP_MAP * cmap_copy_map(CMAP_MAP * dst, CMAP_MAP * src);

int cmap_cmp(CMAP_MAP * map_l, CMAP_MAP * map_r);

CMAP_MAP * cmap_new(CMAP_FN * prototype, CMAP_PROC_CTX * proc_ctx,
  const char * aisle, ...);
CMAP_MAP * cmap_lnew(CMAP_FN * prototype, CMAP_PROC_CTX * proc_ctx,
  const char * aisle, CMAP_LIST * args);

CMAP_MAP * cmap_lfn_proc(CMAP_FN * fn, CMAP_PROC_CTX * proc_ctx,
  CMAP_MAP * map, CMAP_LIST * args);
CMAP_MAP * cmap_fn_proc(CMAP_FN * fn, CMAP_PROC_CTX * proc_ctx, CMAP_MAP * map,
  ...);

CMAP_MAP * cmap_proc(CMAP_MAP * map, const char * key,
  CMAP_PROC_CTX * proc_ctx, ...);
CMAP_MAP * cmap_lproc(CMAP_MAP * map, const char * key,
  CMAP_PROC_CTX * proc_ctx, CMAP_LIST * args);

CMAP_LIST * cmap_to_list(CMAP_PROC_CTX * proc_ctx, const char * aisle, ...);
CMAP_MAP * cmap_to_map(CMAP_PROC_CTX * proc_ctx, const char * aisle, ...);

CMAP_MEM_STATE * cmap_mem_state();

void cmap_delete_aisle(CMAP_PROC_CTX * proc_ctx, const char * aisle);

CMAP_ENV * cmap_env();
void cmap_env_main(CMAP_ENV * env, int argc, char * argv[],
  void (*init)(CMAP_PROC_CTX *));

CMAP_PROC_CTX * cmap_proc_ctx(CMAP_ENV * env);
void cmap_delete_proc_ctx(CMAP_PROC_CTX * proc_ctx);

CMAP_MAP * cmap_global_env(CMAP_PROC_CTX * proc_ctx);
CMAP_MAP * cmap_definitions(CMAP_PROC_CTX * proc_ctx);

#endif
