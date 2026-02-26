
#include "cmap-log.h"

#include <time.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "cmap-kernel.h"
#include "cmap-config.h"

/*******************************************************************************
*******************************************************************************/

static CMAP_LOG log;
static CMAP_LOG * log_ptr = NULL;

static FILE * log_file = NULL;

/*******************************************************************************
*******************************************************************************/

#define CASE(LVL, lvl) case CMAP_LOG_##LVL: return #LVL;

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
  if(cmap_config_log_lvl() <= CMAP_LOG_ERROR)
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

  const char * path = cmap_config_log_path();
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
    log_ptr = cmap_config_log();
    if(log_ptr == NULL)
    {
      log_file_open();

      log.delete = delete;
      log.vlog = vlog;

      log_ptr = &log;
    }
  }
  return log_ptr;
}

/*******************************************************************************
*******************************************************************************/

void cmap_log_delete()
{
  instance() -> delete();
}

/*******************************************************************************
*******************************************************************************/

void cmap_vlog(char lvl, const char * msg, va_list args)
{
  if(lvl >= cmap_config_log_lvl())
  {
    instance() -> vlog(lvl_val(lvl), msg, args);

    if(lvl == CMAP_LOG_FATAL) cmap_kernel_fatal();
  }
}

void cmap_log(char lvl, const char * msg, ...)
{
  va_list args;
  va_start(args, msg);
  cmap_vlog(lvl, msg, args);
  va_end(args);
}

/*******************************************************************************
*******************************************************************************/

#define IMPL(LVL, lvl) \
void cmap_log_v##lvl(const char * msg, va_list args) \
{ \
  cmap_vlog(CMAP_LOG_##LVL, msg, args); \
} \
 \
void cmap_log_##lvl(const char * msg, ...) \
{ \
  va_list args; \
  va_start(args, msg); \
  cmap_vlog(CMAP_LOG_##LVL, msg, args); \
  va_end(args); \
}

CMAP_LOG_LOOP(IMPL)
