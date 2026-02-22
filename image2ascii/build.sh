#!/bin/bash

cc \
    -Wall\
    -Wextra\
    -ggdb\
    -I/home/piero/Documents/programming/LIB/stb/ \
    -o ./main main.c \
    -lm

if [ $# -eq 0 ]; then
  exit 0;
fi

if [ "$1" = "run" ]; then
  ./main
fi
