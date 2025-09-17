
include $(top_srcdir)/src/common.mk

BUILT_SOURCES = \
  cmap-scheduler-blt.c cmap-scheduler-blt.h \
  cmap-job-blt.c cmap-job-blt.h

EXTRA_DIST = cmap-scheduler.cmap cmap-job.cmap

noinst_HEADERS = cmap-scheduler.h

noinst_LTLIBRARIES = libscheduler.la

nodist_libscheduler_la_SOURCES = cmap-scheduler-blt.c cmap-job-blt.c
libscheduler_la_SOURCES = cmap-scheduler.c
libscheduler_la_CFLAGS = $(CMAP_CFLAGS)

CLEANFILES = $(BUILT_SOURCES)

CMAP_SCHEDULER_FILE = $(CMAP_SRC_PATH)/components/scheduler/cmap-scheduler.cmap

cmap-scheduler-blt.c cmap-scheduler-blt.h: $(CMAP_SCHEDULER_FILE) $(CMAP_TOOL)
	$(CMAP_TOOL) build $(CMAP_SCHEDULER_FILE) cmap-scheduler-blt -i

CMAP_JOB_FILE = $(CMAP_SRC_PATH)/components/scheduler/cmap-job.cmap

cmap-job-blt.c cmap-job-blt.h: $(CMAP_JOB_FILE) $(CMAP_TOOL)
	$(CMAP_TOOL) build $(CMAP_JOB_FILE) cmap-job-blt \
	  -i -fcmap_job_blt_create
