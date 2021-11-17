#!/usr/bin/env bash
gcc -o lua.so lua.c $(yed --print-cflags) $(yed --print-ldflags)
