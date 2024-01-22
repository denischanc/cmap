
include $(top_srcdir)/src/common.mk

noinst_HEADERS = \
  cmap-slist-define.h \
  cmap-slist.h \
  cmap-sset-define.h \
  cmap-sset.h \
  cmap-stree-define.h \
  cmap-stree.h

noinst_LTLIBRARIES = libscore.la

libscore_la_SOURCES = \
  cmap-slist.c \
  cmap-sset.c \
  cmap-stree.c
libscore_la_CFLAGS = $(CMAP_CFLAGS)
