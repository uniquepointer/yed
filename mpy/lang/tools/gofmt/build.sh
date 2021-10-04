#!/usr/bin/env bash
gcc -o gofmt.so gofmt.c $(yed --print-cflags) $(yed --print-ldflags)
