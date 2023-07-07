
#include "cmap-parser-part.h"

#include <stdlib.h>
#include <string.h>
#include "cmap-parser-string.h"

/*******************************************************************************
*******************************************************************************/

typedef struct INSTRUCTIONS INSTRUCTIONS;

struct INSTRUCTIONS
{
  char * instructions;
  INSTRUCTIONS * next;
};

/*******************************************************************************
*******************************************************************************/

static char * includes = NULL, * functions = NULL, * main = NULL;

static INSTRUCTIONS * instructions = NULL;

/*******************************************************************************
*******************************************************************************/

static char ** includes_()
{
  if(includes == NULL) includes = strdup("");
  return &includes;
}

/*******************************************************************************
*******************************************************************************/

static char ** functions_()
{
  if(functions == NULL) functions = strdup("");
  return &functions;
}

/*******************************************************************************
*******************************************************************************/

static char ** main_()
{
  if(main == NULL) main = strdup("");
  return &main;
}

/*******************************************************************************
*******************************************************************************/

static char ** instructions_()
{
  return &instructions -> instructions;
}

/*******************************************************************************
*******************************************************************************/

static void push_instructions()
{
  INSTRUCTIONS * tmp = (INSTRUCTIONS *)malloc(sizeof(INSTRUCTIONS));
  tmp -> instructions = strdup("");
  tmp -> next = instructions;
  instructions = tmp;
}

/*******************************************************************************
*******************************************************************************/

static char * pop_instructions()
{
  char * ret = instructions -> instructions;
  INSTRUCTIONS * tmp = instructions;
  instructions = instructions -> next;
  free(tmp);
  return ret;
}

/*******************************************************************************
*******************************************************************************/

const CMAP_PARSER_PART_PUBLIC cmap_parser_part_public =
{
  includes_,
  functions_,
  main_,
  instructions_,
  push_instructions,
  pop_instructions
};
