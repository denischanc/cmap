
#include "cmap-log.h"

#include <time.h>
#include <stdio.h>
#include "cmap-kernel.h"

/*******************************************************************************
*******************************************************************************/

#define LOOP(macro) \
  macro(debug, cmap_log_public.debug) \
  macro(info, cmap_log_public.info) \
  macro(warn, cmap_log_public.warn) \
  macro(error, cmap_log_public.error) \
  macro(fatal, cmap_log_public.fatal)

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

#define FN(name, level) \
static void name(const char * msg, ...) \
{ \
  va_list ap; \
  va_start(ap, msg); \
  vlog(level, msg, ap); \
  va_end(ap); \
}

LOOP(FN)

/*******************************************************************************
*******************************************************************************/

#define SET(name, level) \
  log.name = name;

static CMAP_LOG * init()
{
  if(log_ptr == NULL)
  {
    LOOP(SET)
    log.log = log_;
    log.vlog = vlog;

    log_ptr = &log;
  }
  return log_ptr;
}

static CMAP_LOG * instance()
{
  return log_ptr;
}

/*******************************************************************************
*******************************************************************************/

const CMAP_LOG_PUBLIC cmap_log_public =
{
  "DEBUG",
  "_INFO",
  "_WARN",
  "ERROR",
  "FATAL",
  init,
  instance
};
