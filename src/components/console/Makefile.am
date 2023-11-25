
include $(top_srcdir)/src/common.mk

EXTRA_DIST = cmap-console.cmap

nodist_noinst_HEADERS = cmap-console.h
noinst_HEADERS = cmap-console-ep.h

noinst_LTLIBRARIES = libconsole.la

nodist_libconsole_la_SOURCES = cmap-console.c
libconsole_la_SOURCES = cmap-console-ep.c
libconsole_la_CFLAGS = $(CMAP_CFLAGS)

CLEANFILES = cmap-console.c cmap-console.h

CMAP_CONSOLE_FILE = $(CMAP_SRC_PATH)/components/console/cmap-console.cmap

cmap-console.c cmap-console.h: $(CMAP_CONSOLE_FILE) $(CMAP_TOOL)
	$(CMAP_TOOL) gen $(CMAP_CONSOLE_FILE) cmap-console -i \
	  -fcmap_console_public_create
