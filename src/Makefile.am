
SUBDIRS = util kernel

include_HEADERS = \
  cmap-common.h \
  cmap-fw.h
lib_LTLIBRARIES = libcmap.la

libcmap_la_SOURCES =
libcmap_la_LDFLAGS =
libcmap_la_LIBADD = kernel/libkernel.la util/libutil.la

if CYGWIN
libcmap_la_LDFLAGS += -no-undefined
endif
