#!/usr/bin/env bash
gcc -o nixfmt.so nixfmt.c $(yed --print-cflags) $(yed --print-ldflags)
