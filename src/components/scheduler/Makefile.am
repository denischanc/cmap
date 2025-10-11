
include $(top_srcdir)/src/common.mk
include $(top_srcdir)/src/components/common.mk

EXTRA_DIST = cmap-job.cmap cmap-scheduler.cmap

BUILT_SOURCES = $(EXTRA_DIST:.cmap=-blt.h)

noinst_HEADERS = cmap-scheduler.h

noinst_LTLIBRARIES = libscheduler.la

nodist_libscheduler_la_SOURCES = $(BUILT_SOURCES:.h=.c)
libscheduler_la_SOURCES = cmap-scheduler.c
libscheduler_la_CFLAGS = $(CMAP_CFLAGS)

CLEANFILES = $(BUILT_SOURCES) $(BUILT_SOURCES:.h=.c)
