
#include "cmap-parser-block.h"

#include <stdlib.h>
#include <string.h>
#include "cmap-part.h"
#include "cmap-parser-util.h"
#include "cmap-parser-part.h"
#include "cmap-string.h"

/*******************************************************************************
*******************************************************************************/

static void if_(char * cmp_call)
{
  char * instructions = cmap_part_public.pop_instructions();

  char * else_ = cmap_part_public.is_else_n_rst() ? "else " : "";
  APPEND_INSTRUCTION_ARGS("%sif(%s)", else_, cmp_call);
  free(cmp_call);
  APPEND_INSTRUCTION("{");
  cmap_parser_part_public.append_instructions(instructions);
  free(instructions);
  APPEND_INSTRUCTION("}");
}

static void else_empty()
{
  APPEND_LF();
}

static void else_if()
{
  cmap_part_public.set_else();
}

static void else_()
{
  char * instructions = cmap_part_public.pop_instructions();

  APPEND_INSTRUCTION("else");
  APPEND_INSTRUCTION("{");
  cmap_parser_part_public.append_instructions(instructions);
  free(instructions);
  APPEND_INSTRUCTION("}");
  APPEND_LF();
}

/*******************************************************************************
*******************************************************************************/

static void for_(char * cmp_call, char * iter_call)
{
  char * instructions = cmap_part_public.pop_instructions();

  APPEND_INSTRUCTION_ARGS("for(; %s; %s)", cmp_call, iter_call);
  free(cmp_call); free(iter_call);
  APPEND_INSTRUCTION("{");
  cmap_parser_part_public.append_instructions(instructions);
  free(instructions);
  APPEND_INSTRUCTION("}");
  APPEND_LF();
}

/*******************************************************************************
*******************************************************************************/

static void while_(char * cmp_call)
{
  char * instructions = cmap_part_public.pop_instructions();

  APPEND_INSTRUCTION_ARGS("while(%s)", cmp_call);
  free(cmp_call);
  APPEND_INSTRUCTION("{");
  cmap_parser_part_public.append_instructions(instructions);
  free(instructions);
  APPEND_INSTRUCTION("}");
  APPEND_LF();
}

/*******************************************************************************
*******************************************************************************/

const CMAP_PARSER_BLOCK_PUBLIC cmap_parser_block_public =
{
  if_, else_empty, else_if, else_,
  for_, while_
};
