
include $(top_srcdir)/src/common.mk

EXTRA_DIST = cmap-scheduler.cmap cmap-job.cmap

nodist_noinst_HEADERS = cmap-scheduler-blt.h cmap-job-blt.h
noinst_HEADERS = cmap-scheduler.h

noinst_LTLIBRARIES = libscheduler.la

nodist_libscheduler_la_SOURCES = cmap-scheduler-blt.c cmap-job-blt.c
libscheduler_la_SOURCES = cmap-scheduler.c
libscheduler_la_CFLAGS = $(CMAP_CFLAGS)

cmap-scheduler.c: cmap-scheduler-blt.h

cmap-scheduler-blt.c: cmap-job-blt.h

CLEANFILES = $(nodist_libscheduler_la_SOURCES) $(nodist_noinst_HEADERS)

CMAP_SCHEDULER_FILE = $(CMAP_SRC_PATH)/components/scheduler/cmap-scheduler.cmap
CMAP_JOB_FILE = $(CMAP_SRC_PATH)/components/scheduler/cmap-job.cmap

cmap-scheduler-blt.c cmap-scheduler-blt.h: $(CMAP_SCHEDULER_FILE) $(CMAP_TOOL)
	$(CMAP_TOOL) build $(CMAP_SCHEDULER_FILE) cmap-scheduler-blt -i

cmap-job-blt.c cmap-job-blt.h: $(CMAP_JOB_FILE) $(CMAP_TOOL)
	$(CMAP_TOOL) build $(CMAP_JOB_FILE) cmap-job-blt \
	  -i -fcmap_job_blt_public_create
