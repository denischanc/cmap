
include $(top_srcdir)/src/common.mk

CMAP_KERNEL_SRC_PATH = $(CMAP_SRC_PATH)/kernel

CMAP_KERNEL_CFLAGS = \
  -I$(CMAP_SRC_PATH) \
  -I$(CMAP_KERNEL_SRC_PATH) \
  -I$(CMAP_KERNEL_SRC_PATH)/core/mem \
  -I$(CMAP_KERNEL_SRC_PATH)/core/map
