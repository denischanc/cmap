
include $(top_srcdir)/src/common.mk

include_HEADERS = cmap-core.h

noinst_HEADERS = \
  cmap-fn.h \
  cmap-int.h \
  cmap-list-define.h \
  cmap-list.h \
  cmap-map-define.h \
  cmap-map.h \
  cmap-mem-define.h \
  cmap-mem.h \
  cmap-string.h

noinst_LTLIBRARIES = libcore.la

libcore_la_SOURCES = \
  cmap-mem.c \
  cmap-map.c \
  cmap-list.c \
  cmap-fn.c \
  cmap-string.c \
  cmap-int.c
libcore_la_CFLAGS = $(CMAP_KERNEL_CFLAGS)
