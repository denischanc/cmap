
include $(top_srcdir)/src/common.mk

noinst_HEADERS = \
  cmap-cmp-define.h \
  cmap-cmp-double-handler.h \
  cmap-cmp.h \
  cmap-cmp-handler.h \
  cmap-cmp-int-handler.h \
  cmap-cmp-string-handler.h

noinst_LTLIBRARIES = libcmp.la

libcmp_la_SOURCES = \
  cmap-cmp.c \
  cmap-cmp-double-handler.c \
  cmap-cmp-int-handler.c \
  cmap-cmp-string-handler.c
libcmp_la_CFLAGS = $(CMAP_CFLAGS)
