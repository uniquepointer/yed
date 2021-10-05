CFLAGS += $(shell yed --print-cflags) -Wall -Wextra
CFLAGS += $(shell yed --print-ldflags)
install:
	gcc $(CFLAGS) gofmt.c -o gofmt.so
	cp ./gofmt.so ~/.config/yed/mpy/plugins/lang/tools/.
