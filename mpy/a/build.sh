#!/usr/bin/env bash
gcc -o a.so a.c $(yed --print-cflags) $(yed --print-ldflags)
