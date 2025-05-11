
include $(top_srcdir)/src/tool/common.mk

noinst_HEADERS = \
  cmap-part-ctx-define.h \
  cmap-part-ctx.h \
  cmap-part-dirty.h \
  cmap-part-kv.h \
  cmap-part-params.h \
  cmap-part-var.h

noinst_LTLIBRARIES = libpart.la

libpart_la_SOURCES = \
  cmap-part-ctx.c \
  cmap-part-dirty.c \
  cmap-part-kv.c \
  cmap-part-params.c \
  cmap-part-var.c
libpart_la_CFLAGS = $(CMAP_TOOL_CFLAGS)
