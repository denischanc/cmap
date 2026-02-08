
include $(top_srcdir)/src/common.mk

SUBDIRS = console scheduler cli

EXTRA_DIST = cmap-cmap.cmap

BUILT_SOURCES = $(EXTRA_DIST:.cmap=-blt.h)

noinst_HEADERS = cmap-cmap.h

noinst_LTLIBRARIES = libcomponents.la

nodist_libcomponents_la_SOURCES = $(BUILT_SOURCES:.h=.c)
libcomponents_la_SOURCES = cmap-cmap.c
libcomponents_la_LIBADD = \
  console/libconsole.la \
  scheduler/libscheduler.la \
  cli/libcli.la

CLEANFILES = $(BUILT_SOURCES) $(BUILT_SOURCES:.h=.c)

cmap-cmap-blt.h: $(srcdir)/cmap-cmap.cmap $(CMAP_TOOL)
	$(CMAP_TOOL) build $< $(@:.h=) --relative-inc

cmap-cmap-blt.h: \
  $(srcdir)/console/cmap-console.cmap \
  $(srcdir)/scheduler/cmap-job.cmap \
  $(srcdir)/scheduler/cmap-scheduler.cmap
