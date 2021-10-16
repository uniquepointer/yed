CFLAGS += $(shell yed --print-cflags) -Wall -Wextra
CFLAGS += $(shell yed --print-ldflags)
install:
	gcc $(CFLAGS) clangfmt.c -o clangfmt.so
	cp ./clangfmt.so ~/.config/yed/mpy/plugins/lang/tools/.
