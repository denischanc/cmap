
include $(top_srcdir)/src/common.mk

CMAP_KERNEL_CFLAGS = \
  -I$(CMAP_SRC_PATH) \
  -I$(CMAP_KERNEL_SRC_PATH)/core/mem
