
include $(top_srcdir)/src/common.mk

BUILT_SOURCES = cmap-scheduler.h cmap-job.h

EXTRA_DIST = cmap-scheduler.cmap cmap-job.cmap

nodist_noinst_HEADERS = $(BUILT_SOURCES)
noinst_HEADERS = cmap-scheduler-ep.h

noinst_LTLIBRARIES = libscheduler.la

nodist_libscheduler_la_SOURCES = cmap-scheduler.c cmap-job.c
libscheduler_la_SOURCES = cmap-scheduler-ep.c
libscheduler_la_CFLAGS = $(CMAP_CFLAGS)

CLEANFILES = cmap-scheduler.c cmap-job.c $(BUILT_SOURCES)

CMAP_SCHEDULER_FILE = $(CMAP_SRC_PATH)/components/scheduler/cmap-scheduler.cmap
CMAP_JOB_FILE = $(CMAP_SRC_PATH)/components/scheduler/cmap-job.cmap

cmap-scheduler.c cmap-scheduler.h: $(CMAP_SCHEDULER_FILE) $(CMAP_MAIN)
	$(CMAP_MAIN) gen $(CMAP_SCHEDULER_FILE) cmap-scheduler -i

cmap-job.c cmap-job.h: $(CMAP_JOB_FILE) $(CMAP_MAIN)
	$(CMAP_MAIN) gen $(CMAP_JOB_FILE) cmap-job -i
