
include $(top_srcdir)/src/common.mk

SUBDIRS = console scheduler cli

BUILT_SOURCES = cmap-cmap-blt.c cmap-cmap-blt.h

EXTRA_DIST = cmap-cmap.cmap

noinst_HEADERS = cmap-cmap.h

noinst_LTLIBRARIES = libcomponents.la

nodist_libcomponents_la_SOURCES = cmap-cmap-blt.c
libcomponents_la_SOURCES = cmap-cmap.c
libcomponents_la_CFLAGS = $(CMAP_CFLAGS)
libcomponents_la_LIBADD = \
  console/libconsole.la \
  scheduler/libscheduler.la \
  cli/libcli.la

CMAP_CMAP_FILE = $(CMAP_SRC_PATH)/components/cmap-cmap.cmap

cmap-cmap-blt.c cmap-cmap-blt.h: $(CMAP_CMAP_FILE) $(CMAP_TOOL)
	$(CMAP_TOOL) build $(CMAP_CMAP_FILE) cmap-cmap-blt \
	  -i -fcmap_cmap_blt_create
