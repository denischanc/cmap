
#include "cmap-log.h"

#include <time.h>
#include <stdio.h>
#include "cmap-kernel.h"

/*******************************************************************************
*******************************************************************************/

#define CONST_VAR(LVL, lvl, i) const char CMAP_LOG_##LVL = i;

CMAP_LOG_LOOP(CONST_VAR)

/*******************************************************************************
*******************************************************************************/

static CMAP_LOG log = {};
static CMAP_LOG * log_ptr = NULL;

/*******************************************************************************
*******************************************************************************/

#define CASE(LVL, lvl, i) case i: return #LVL;

static const char * lvl_val(char lvl)
{
  switch(lvl)
  {
    CMAP_LOG_LOOP(CASE)
  }
  return "UNDEFINED";
}

/*******************************************************************************
*******************************************************************************/

static void vlog_(char lvl, const char * msg, va_list args)
{
  if(lvl >= CMAP_KERNEL_INSTANCE -> cfg() -> log.lvl)
    CMAP_KERNEL_INSTANCE -> log() -> vlog(lvl_val(lvl), msg, args);
}

static void log_(char lvl, const char * msg, ...)
{
  if(lvl >= CMAP_KERNEL_INSTANCE -> cfg() -> log.lvl)
  {
    va_list args;
    va_start(args, msg);
    CMAP_KERNEL_INSTANCE -> log() -> vlog(lvl_val(lvl), msg, args);
    va_end(args);
  }
}

/*******************************************************************************
*******************************************************************************/

#define IMPL(LVL, lvl, i) \
static void v##lvl(const char * msg, va_list args) \
{ \
  vlog_(i, msg, args); \
} \
 \
static void lvl(const char * msg, ...) \
{ \
  va_list args; \
  va_start(args, msg); \
  vlog_(i, msg, args); \
  va_end(args); \
}

CMAP_LOG_LOOP(IMPL)

/*******************************************************************************
*******************************************************************************/

static void vlog(const char * level, const char * msg, va_list args)
{
  time_t t = time(NULL);
  struct tm * tm = localtime(&t);

  FILE * f = stderr;
  fprintf(f, "[%d-%02d-%02d_%02d:%02d:%02d][%s]  ",
    tm -> tm_year + 1900, tm -> tm_mon + 1, tm -> tm_mday,
    tm -> tm_hour, tm -> tm_min, tm -> tm_sec, level);
  vfprintf(f, msg, args);
  fprintf(f, "\n");
}

static CMAP_LOG * instance()
{
  if(log_ptr == NULL)
  {
    log.vlog = vlog;

    log_ptr = &log;
  }
  return log_ptr;
}

/*******************************************************************************
*******************************************************************************/

#define SET(LVL, lvl, i) lvl, v##lvl,

const CMAP_LOG_PUBLIC cmap_log_public =
{
  instance,
  log_, vlog_,
  CMAP_LOG_LOOP(SET)
};
