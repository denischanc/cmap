
include $(top_srcdir)/src/common.mk

include_cmap_HEADERS = \
  cmap-env-type.h \
  cmap-kernel-type.h \
  cmap-log-define-ext.h \
  cmap-log-ext.h \
  cmap-log-type.h \
  cmap-mem-type.h \
  cmap-proc-ctx-type.h

noinst_HEADERS = \
  cmap-env.h \
  cmap-global-env.h \
  cmap-kernel-define.h \
  cmap-kernel.h \
  cmap-log-define.h \
  cmap-log.h \
  cmap-mem-define.h \
  cmap-mem.h \
  cmap-pool-define.h \
  cmap-pool.h \
  cmap-pool-handler-define.h \
  cmap-pool-handler.h \
  cmap-proc-ctx.h \
  cmap-refsstore.h

noinst_LTLIBRARIES = libkernel.la

libkernel_la_SOURCES = \
  cmap-env.c \
  cmap-global-env.c \
  cmap-kernel.c \
  cmap-log.c \
  cmap-mem.c \
  cmap-pool.c \
  cmap-pool-handler.c \
  cmap-proc-ctx.c \
  cmap-refsstore.c
libkernel_la_CFLAGS = $(CMAP_CFLAGS)
