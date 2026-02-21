#!/bin/bash

cc \
    -Wall\
    -Wextra\
    -ggdb\
    -I/home/piero/Documents/programming/LIB/raylib/Release/include/ \
    '-I/home/piero/Documents/programming/LIB/sort/' \
    -o ./main main.c \
    -L/home/piero/Documents/programming/LIB/raylib/Release/lib/ \
    -l:libraylib.a \
    -DINCLUDE_SORT \
    -lm

if [ $# -eq 0 ]; then
  exit 0;
fi

if [ "$1" = "run" ]; then
  ./main
fi
