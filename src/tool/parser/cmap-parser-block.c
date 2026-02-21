
#include "cmap-parser-block.h"

#include <stdlib.h>
#include <string.h>
#include "cmap-part.h"
#include "cmap-parser-util.h"
#include "cmap-parser-part.h"
#include "cmap-string.h"
#include "cmap-stack-define.h"

/*******************************************************************************
*******************************************************************************/

CMAP_STACK_DEF(ELSE, else, char)

CMAP_STACK_ELSE * elses = NULL;

/*******************************************************************************
*******************************************************************************/

void cmap_parser_block_init_if()
{
  cmap_stack_else_push(&elses, (1 == 0));
}

void cmap_parser_block_if(char * cmp_call)
{
  char * instructions = cmap_part_pop_instructions();

  char * else_ = cmap_stack_else_pop(&elses) ? "else " : "";
  APPEND_INSTRUCTION_ARGS("%sif(%s)", else_, cmp_call);
  free(cmp_call);
  APPEND_INSTRUCTION("{");
  cmap_parser_part_append_instructions(instructions);
  free(instructions);
  APPEND_INSTRUCTION("}");
}

void cmap_parser_block_else_empty()
{
  APPEND_LF();
}

void cmap_parser_block_else_if()
{
  cmap_stack_else_push(&elses, (1 == 1));
}

void cmap_parser_block_else()
{
  char * instructions = cmap_part_pop_instructions();

  APPEND_INSTRUCTION("else");
  APPEND_INSTRUCTION("{");
  cmap_parser_part_append_instructions(instructions);
  free(instructions);
  APPEND_INSTRUCTION("}");
  APPEND_LF();
}

/*******************************************************************************
*******************************************************************************/

void cmap_parser_block_for(char * cmp_call, char * iter_call)
{
  char * instructions = cmap_part_pop_instructions();

  APPEND_INSTRUCTION_ARGS("for(; %s; %s)", cmp_call, iter_call);
  free(cmp_call); free(iter_call);
  APPEND_INSTRUCTION("{");
  cmap_parser_part_append_instructions(instructions);
  free(instructions);
  APPEND_INSTRUCTION("}");
  APPEND_LF();
}

/*******************************************************************************
*******************************************************************************/

void cmap_parser_block_while(char * cmp_call)
{
  char * instructions = cmap_part_pop_instructions();

  APPEND_INSTRUCTION_ARGS("while(%s)", cmp_call);
  free(cmp_call);
  APPEND_INSTRUCTION("{");
  cmap_parser_part_append_instructions(instructions);
  free(instructions);
  APPEND_INSTRUCTION("}");
  APPEND_LF();
}

/*******************************************************************************
*******************************************************************************/

void cmap_parser_block_clean()
{
  while(elses != NULL) cmap_stack_else_pop(&elses);
}
