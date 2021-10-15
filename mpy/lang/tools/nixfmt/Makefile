CFLAGS += $(shell yed --print-cflags) -Wall -Wextra
CFLAGS += $(shell yed --print-ldflags)
install:
	gcc $(CFLAGS) nixfmt.c -o nixfmt.so
	cp ./nixfmt.so ~/.config/yed/mpy/plugins/lang/tools/.
