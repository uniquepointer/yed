CFLAGS += $(shell yed --print-cflags)
CFLAGS += $(shell yed --print-ldflags) -Wall -Wextra
install:
	gcc $(CFLAGS) alternate.c -o alternate.so
	cp ./alternate.so ~/.config/yed/mpy/plugins
