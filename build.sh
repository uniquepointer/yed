#!/usr/bin/env bash
gcc -o clangfmt.so clangfmt.c $(yed --print-cflags) $(yed --print-ldflags)
