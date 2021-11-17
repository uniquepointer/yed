#!/usr/bin/env bash
gcc -o confirm-quit.so confirm-quit.c $(yed --print-cflags) $(yed --print-ldflags)
