CFLAGS += $(shell yed --print-cflags)
CFLAGS += $(shell yed --print-ldflags)
install:
	gcc $(CFLAGS) midnight.c -o midnight.so
	cp ./midnight.so ~/.config/yed/mpy/plugins/styles
