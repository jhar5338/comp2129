#!/usr/bin/env sh

clang -g -Wall -Werror -std=gnu11 -fsanitize=address snapshot.c -o snapshot
