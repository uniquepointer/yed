#!/usr/bin/env bash
gcc -o word_wrap.so word_wrap.c $(yed --print-cflags) $(yed --print-ldflags)
