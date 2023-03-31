
#include "cmap-log.h"

#include <time.h>
#include <stdio.h>
#include "cmap-kernel.h"

/*******************************************************************************
*******************************************************************************/

#define LEVEL(LVL, lvl) const char * CMAP_LOG_##LVL = #LVL;

CMAP_LOG_LOOP(LEVEL)

/*******************************************************************************
*******************************************************************************/

static CMAP_LOG log = {};
static CMAP_LOG * log_ptr = NULL;

/*******************************************************************************
*******************************************************************************/

static void vlog(const char * level, const char * msg, va_list ap)
{
  time_t t = time(NULL);
  struct tm * tm = localtime(&t);

  FILE * f = stderr;
  fprintf(f, "[%d-%02d-%02d_%02d:%02d:%02d][%s]  ",
    tm -> tm_year + 1900, tm -> tm_mon + 1, tm -> tm_mday,
    tm -> tm_hour, tm -> tm_min, tm -> tm_sec, level);
  vfprintf(f, msg, ap);
  fprintf(f, "\n");
}

/*******************************************************************************
*******************************************************************************/

static void log_(const char * level, const char * msg, ...)
{
  va_list ap;
  va_start(ap, msg);
  vlog(level, msg, ap);
  va_end(ap);
}

/*******************************************************************************
*******************************************************************************/

#define FN(LVL, lvl) \
static void v##lvl(const char * msg, va_list ap) \
{ \
  cmap_kernel_public.log() -> vlog(CMAP_LOG_##LVL, msg, ap); \
} \
 \
static void lvl(const char * msg, ...) \
{ \
  va_list ap; \
  va_start(ap, msg); \
  v##lvl(msg, ap); \
  va_end(ap); \
}

CMAP_LOG_LOOP(FN)

/*******************************************************************************
*******************************************************************************/

static CMAP_LOG * instance()
{
  if(log_ptr == NULL)
  {
    log.log = log_;
    log.vlog = vlog;

    log_ptr = &log;
  }
  return log_ptr;
}

/*******************************************************************************
*******************************************************************************/

#define SET(LVL, lvl) lvl, v##lvl,

const CMAP_LOG_PUBLIC cmap_log_public =
{
  instance,
  CMAP_LOG_LOOP(SET)
};
