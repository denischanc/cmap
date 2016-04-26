
SUBDIRS = util kernel fw

include_HEADERS = \
  cmap-common.h

lib_LTLIBRARIES = libcmap.la

libcmap_la_SOURCES =
libcmap_la_LDFLAGS =
libcmap_la_LIBADD = \
  util/libutil.la \
  kernel/libkernel.la \
  fw/libfw.la

if CYGWIN
libcmap_la_LDFLAGS += -no-undefined
endif
