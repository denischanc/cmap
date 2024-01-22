
include $(top_srcdir)/src/common.mk

noinst_HEADERS = \
  cmap-assert-define.h \
  cmap-assert.h \
  cmap-consumedtime.h \
  cmap-util.h

noinst_LTLIBRARIES = libutil.la

libutil_la_SOURCES = \
  cmap-assert.c \
  cmap-util.c
if CONSUMED_TIME
libutil_la_SOURCES += cmap-consumedtime.c
endif
libutil_la_CFLAGS = $(CMAP_CFLAGS)
