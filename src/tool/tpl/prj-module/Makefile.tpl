
.PHONY: clean

CMAP_TOOL = @CMAP_BINDIR@/cmap-tool

.cmap/watch-mem.so: watch-mem.cmap watch-mem-fn.c watch-mem-fn.h $(CMAP_TOOL)
	$(CMAP_TOOL) compile-module $<

clean:
	rm -rf .cmap
