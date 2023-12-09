
.PHONY: clean

CMAP_TOOL = @CMAP_INSTALL_BINDIR@/cmap-tool

CFLAGS = -Wall -I@CMAP_INSTALL_INCLUDEDIR@
LDFLAGS = -L@CMAP_INSTALL_LIBDIR@ -lcmap

snake: random-fn.c random.c main.c snake.c screen.c
	gcc $(CFLAGS) $(LDFLAGS) -o $@ $^

random.c: random.cmap $(CMAP_TOOL)
	$(CMAP_TOOL) build $< $(@:.c=) --fn=random_global_env

main.c: main.cmap $(CMAP_TOOL)
	$(CMAP_TOOL) build $< $(@:.c=) --only-c --add-main --fn=main_

%.c: %.cmap $(CMAP_TOOL)
	$(CMAP_TOOL) build $< $(@:.c=)

clean:
	rm -f snake main.c random.c random.h snake.c snake.h
	rm -f screen.c screen.h
