#!/usr/bin/env bash
gcc -o midnight.so midnight.c $(yed --print-cflags) $(yed --print-ldflags)
