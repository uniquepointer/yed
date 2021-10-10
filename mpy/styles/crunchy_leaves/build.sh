#!/usr/bin/env bash
gcc -o crunchy_leaves.so crunchy_leaves.c $(yed --print-cflags) $(yed --print-ldflags)
