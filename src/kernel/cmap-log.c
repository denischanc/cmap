
#include "cmap-log.h"

#include <time.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "cmap-kernel.h"

/*******************************************************************************
*******************************************************************************/

#define CONST_VAR(LVL, lvl, i) const char CMAP_LOG_##LVL = i;

CMAP_LOG_LOOP(CONST_VAR)

/*******************************************************************************
*******************************************************************************/

static CMAP_LOG log;
static CMAP_LOG * log_ptr = NULL;

static FILE * log_file = NULL;

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

static void log_(char lvl, const char * msg, ...)
{
  CMAP_KERNEL * kernel = CMAP_KERNEL_INSTANCE;
  if(lvl >= kernel -> cfg() -> log.lvl)
  {
    va_list args;
    va_start(args, msg);
    kernel -> log() -> vlog(lvl_val(lvl), msg, args);
    va_end(args);

    if(lvl == CMAP_LOG_FATAL) kernel -> fatal();
  }
}

static void vlog_(char lvl, const char * msg, va_list args)
{
  CMAP_KERNEL * kernel = CMAP_KERNEL_INSTANCE;
  if(lvl >= kernel -> cfg() -> log.lvl)
  {
    kernel -> log() -> vlog(lvl_val(lvl), msg, args);

    if(lvl == CMAP_LOG_FATAL) kernel -> fatal();
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

  fprintf(log_file, "[%d-%02d-%02d_%02d:%02d:%02d][%s]  ",
    tm -> tm_year + 1900, tm -> tm_mon + 1, tm -> tm_mday,
    tm -> tm_hour, tm -> tm_min, tm -> tm_sec, level);
  vfprintf(log_file, msg, args);
  fprintf(log_file, "\n");
}

static void error_(const char * msg, ...)
{
  CMAP_KERNEL * kernel = CMAP_KERNEL_INSTANCE;
  if(kernel -> cfg() -> log.lvl <= CMAP_LOG_ERROR)
  {
    va_list args;
    va_start(args, msg);
    vlog(lvl_val(CMAP_LOG_ERROR), msg, args);
    va_end(args);
  }
}

/*******************************************************************************
*******************************************************************************/

static void log_file_open()
{
  log_file = stderr;

  const char * path = CMAP_KERNEL_INSTANCE -> cfg() -> log.path;
  if(path == NULL) return;

  log_file = fopen(path, "a");
  if(log_file == NULL)
  {
    log_file = stderr;
    error_("%s : %s", path, strerror(errno));
  }
}

static void log_file_close()
{
  if(log_file != stderr) fclose(log_file);
}

/*******************************************************************************
*******************************************************************************/

static void delete()
{
  log_file_close();
}

static CMAP_LOG * instance()
{
  if(log_ptr == NULL)
  {
    log_file_open();

    log.delete = delete;
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
