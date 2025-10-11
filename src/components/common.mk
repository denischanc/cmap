
%-blt.h: $(srcdir)/%.cmap $(CMAP_TOOL)
	$(CMAP_TOOL) build $< $(@:.h=) -i
