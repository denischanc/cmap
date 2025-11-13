
.PHONY: clean

CMAP_TOOL = @CMAP_BINDIR@/cmap-tool

CFLAGS = -Wall -I@CMAP_INCLUDEDIR@
LDFLAGS = -L@CMAP_LIBDIR@ -lcmap

snake: random-fn.c main.c
	gcc $(CFLAGS) $(LDFLAGS) -o $@ $^

main.c: main.cmap $(CMAP_TOOL)
	$(CMAP_TOOL) build $< $(@:.c=) --only-c --add-main --fn=main_

main.c: random.cmap screen.cmap snake.cmap

clean:
	rm -f snake main.c
