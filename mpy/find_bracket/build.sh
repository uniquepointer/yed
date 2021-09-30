#!/bin/bash
gcc -o find_bracket.so find_bracket.c $(yed --print-cflags) $(yed --print-ldflags)
