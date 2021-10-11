#!/usr/bin/env bash
gcc -o git_variables.so git_variables.c $(yed --print-cflags) $(yed --print-ldflags)
