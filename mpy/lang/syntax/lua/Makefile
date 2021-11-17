CFLAGS += $(shell yed --print-cflags)
CFLAGS += $(shell yed --print-ldflags)
install:
	gcc $(CFLAGS) lua.c -o lua.so
	cp ./lua.so ~/.config/yed/mpy/plugins/lang/syntax/.
