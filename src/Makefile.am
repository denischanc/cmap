
SUBDIRS = util kernel fw

include_HEADERS = \
  cmap-common-mem.h \
  cmap-common-map.h \
  cmap-common-list.h \
  cmap-common-kernel.h \
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
