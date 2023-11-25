#ifndef __CMAP_PART_H__
#define __CMAP_PART_H__

#include "cmap-part-define.h"
#include "cmap-kv.h"

#define SPACE "  "

extern const char CMAP_PART_CTX_NATURE_DFT;
extern const char CMAP_PART_CTX_NATURE_FN;

/*******************************************************************************
=====[[ .c ]]=====
includes
functions
  instructions
main
  instructions

=====[[ .h ]]=====
headers
*******************************************************************************/
typedef struct
{
  void (*clean)();

  CMAP_PART_LOOP(CMAP_PART_DECL)

  void (*new_ctx)(char nature);
  void (*push_instructions)();
  char ** (*instructions)();
  void (*add_instruction)(const char * instruction);
  void (*add_lf)();
  void (*prepend_instruction)(const char * instruction);
  char (*is_definitions)();
  char (*is_global_env)();
  CMAP_KV * (*name2map)();
  char * (*pop_instructions)();

  void (*return_)();
  char (*is_return)();
  void (*return_fn)();
  char (*is_return_fn)();

  void (*add_include)(const char * name, char is_relative);
} CMAP_PART_PUBLIC;

extern const CMAP_PART_PUBLIC cmap_part_public;

#endif
