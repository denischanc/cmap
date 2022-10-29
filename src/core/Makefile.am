
include $(top_srcdir)/src/common.mk

noinst_HEADERS = \
  cmap-mem.h \
  cmap-mem-define.h \
  cmap-map.h \
  cmap-map-define.h \
  cmap-list.h \
  cmap-list-define.h \
  cmap-fn.h \
  cmap-fn-define.h \
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
