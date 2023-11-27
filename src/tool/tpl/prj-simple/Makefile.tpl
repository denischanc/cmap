
.PHONY: clean

CMAP_TOOL = @CMAP_INSTALL_BINDIR@/cmap-tool

CFLAGS = -Wall -I@CMAP_INSTALL_INCLUDEDIR@
LDFLAGS = -L@CMAP_INSTALL_LIBDIR@ -lcmap

hello-world: hello-world.c
	gcc $(CFLAGS) $(LDFLAGS) -o $@ $^

hello-world.c: hello-world.cmap $(CMAP_TOOL)
	$(CMAP_TOOL) build $< $(@:.c=) --only-c --add-main

clean:
	rm -f hello-world hello-world.c
