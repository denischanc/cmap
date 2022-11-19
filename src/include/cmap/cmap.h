#ifndef ___CMAP_H___
#define ___CMAP_H___

#include <stdarg.h>
#include <stdint.h>

#include <cmap/kernel.h>
#include <cmap/core.h>
#include <cmap/fn.h>
#include <cmap/define.h>
#include <cmap/parser-util.h>

void cmap_bootstrap(CMAP_KERNEL_CFG * cfg);
int cmap_main(int argc, char * argv[], CMAP_MAP * definitions,
  const char * impl);
void cmap_exit(int ret);
void cmap_fatal();

CMAP_MAP * cmap_map(const char * aisle);
CMAP_LIST * cmap_list(int size_inc, const char * aisle);
CMAP_FN * cmap_fn(CMAP_FN_TPL process, const char * aisle);
CMAP_STRING * cmap_string(const char * val, int size_inc, const char * aisle);
CMAP_INT * cmap_int(int64_t val, const char * aisle);
CMAP_DOUBLE * cmap_double(double val, const char * aisle);
CMAP_PTR * cmap_ptr(int size, const char * aisle);

const char * cmap_nature(CMAP_MAP * map);
CMAP_MAP * cmap_delete(CMAP_MAP * map);
void cmap_set(CMAP_MAP * map, const char * key, CMAP_MAP * val);
CMAP_MAP * cmap_get(CMAP_MAP * map, const char * key);

void cmap_list_set(CMAP_LIST * list, int i, CMAP_MAP * val);
CMAP_MAP * cmap_list_get(CMAP_LIST * list, int i);

void cmap_int_set(CMAP_INT * i, int64_t val);
int64_t cmap_int_get(CMAP_INT * i);

void cmap_double_set(CMAP_DOUBLE * d, double val);
double cmap_double_get(CMAP_DOUBLE * d);

void * cmap_ptr_get(CMAP_PTR * ptr);

const char * cmap_string_val(CMAP_STRING * string);

void cmap_set_split(CMAP_MAP * map, const char * keys, CMAP_MAP * val);
CMAP_MAP * cmap_get_split(CMAP_MAP * map, const char * keys);

CMAP_MAP * cmap_new(CMAP_FN * prototype, const char * aisle, ...);
CMAP_MAP * cmap_lnew(CMAP_FN * prototype, const char * aisle, CMAP_LIST * args);

CMAP_MAP * cmap_fn_proc(CMAP_FN * fn, CMAP_MAP * map, ...);

CMAP_MAP * cmap_proc(CMAP_MAP * map, const char * key, ...);
CMAP_MAP * cmap_lproc(CMAP_MAP * map, const char * key, CMAP_LIST * args);

CMAP_MAP * cmap_proc_split(CMAP_MAP * map, const char * keys, ...);
CMAP_MAP * cmap_lproc_split(CMAP_MAP * map, const char * keys,
  CMAP_LIST * args);

CMAP_MAP * cmap_proc_chain(CMAP_MAP * map, ...);

CMAP_LIST * cmap_to_list(const char * aisle, ...);
CMAP_MAP * cmap_to_map(const char * aisle, ...);

CMAP_MEM_STATE * cmap_mem_state();

void cmap_delete_aisle(const char * aisle);

void cmap$$(CMAP_MAP * definitions, const char * impl, SCANNER_NODE ** chain);

#endif
