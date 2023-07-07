
include $(top_srcdir)/src/common.mk

SUBDIRS = \
  parser \
  parser2 \
  util \
  core \
  kernel \
  components

include_cmap_HEADERS = \
  cmap-ext.h \
  cmap-define-ext.h

noinst_HEADERS = \
  cmap.h \
  cmap-define.h

lib_LTLIBRARIES = libcmap.la

libcmap_la_SOURCES = cmap.c
libcmap_la_CFLAGS = $(CMAP_CFLAGS)
libcmap_la_LDFLAGS = -version-info 1:0:0
libcmap_la_LIBADD = \
  parser/libparser.la \
  util/libutil.la \
  core/libcore.la \
  kernel/libkernel.la \
  components/libcomponents.la

if CYGWIN
libcmap_la_LDFLAGS += -no-undefined
endif
