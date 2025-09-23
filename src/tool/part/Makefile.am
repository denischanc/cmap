
include $(top_srcdir)/src/tool/common.mk

noinst_HEADERS = \
  cmap-part-affected.h \
  cmap-part-ctx-define.h \
  cmap-part-ctx.h \
  cmap-part-kv.h \
  cmap-part-name2map.h \
  cmap-part-var.h

noinst_LTLIBRARIES = libpart.la

libpart_la_SOURCES = \
  cmap-part-affected.c \
  cmap-part-ctx.c \
  cmap-part-kv.c \
  cmap-part-name2map.c \
  cmap-part-var.c
libpart_la_CFLAGS = $(CMAP_TOOL_CFLAGS)
