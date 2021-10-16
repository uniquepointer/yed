#!/usr/bin/env bash
gcc -o alternate.so alternate.c $(yed --print-cflags) $(yed --print-ldflags)
