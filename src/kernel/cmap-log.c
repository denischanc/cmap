
#include "cmap-log.h"

#include <time.h>
#include <stdio.h>
#include "cmap-kernel.h"

/*******************************************************************************
*******************************************************************************/

const char * CMAP_LOG_DEBUG = "DEBUG";
const char * CMAP_LOG_INFO = "_INFO";
const char * CMAP_LOG_WARN = "_WARN";
const char * CMAP_LOG_ERROR = "ERROR";
const char * CMAP_LOG_FATAL = "FATAL";

/*******************************************************************************
*******************************************************************************/

#define LOOP(macro) \
  macro(debug, CMAP_LOG_DEBUG) \
  macro(info, CMAP_LOG_INFO) \
  macro(warn, CMAP_LOG_WARN) \
  macro(error, CMAP_LOG_ERROR) \
  macro(fatal, CMAP_LOG_FATAL)

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

static void _log(const char * level, const char * msg, ...)
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
  log -> name = name;

void cmap_log_init(CMAP_LOG * log)
{
  LOOP(SET)
  log -> log = _log;
  log -> vlog = vlog;
}
