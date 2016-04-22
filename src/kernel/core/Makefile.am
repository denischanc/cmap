
include $(top_srcdir)/src/common.mk

include_HEADERS = \
  cmap-mem.h \
  cmap-core.h \
  cmap-map.h \
  cmap-list.h \
  cmap-fn.h \
  cmap-string.h \
  cmap-int.h

noinst_LTLIBRARIES = libcore.la

libcore_la_SOURCES = \
  cmap-mem.c \
  cmap-map.c \
  cmap-list.c \
  cmap-fn.c \
  cmap-string.c \
  cmap-int.c
libcore_la_CFLAGS = $(CMAP_KERNEL_CFLAGS)
