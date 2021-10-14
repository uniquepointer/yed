CFLAGS += $(shell yed --print-cflags)
CFLAGS += $(shell yed --print-ldflags) -Wall -Wextra
install:
	gcc $(CFLAGS) a.c -o a.so
	cp ./a.so ~/.config/yed/mpy/plugins
