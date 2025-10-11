
include $(top_srcdir)/src/common.mk
include $(top_srcdir)/src/components/common.mk

BUILT_SOURCES = \
  cmap-scheduler-blt.c cmap-scheduler-blt.h \
  cmap-job-blt.c cmap-job-blt.h

EXTRA_DIST = cmap-scheduler.cmap cmap-job.cmap

noinst_HEADERS = cmap-scheduler.h

noinst_LTLIBRARIES = libscheduler.la

nodist_libscheduler_la_SOURCES = cmap-scheduler-blt.c cmap-job-blt.c
libscheduler_la_SOURCES = cmap-scheduler.c
libscheduler_la_CFLAGS = $(CMAP_CFLAGS)

CLEANFILES = $(BUILT_SOURCES)
