#!/bin/bash

cc -Wall -Wextra -ggdb -I./raylib-5.5_linux_amd64/include/ -o ./main main.c -L./raylib-5.5_linux_amd64/lib/ -l:libraylib.a -lm

if [ $# -eq 0 ]; then
  exit 0;
fi

if [ "$1" = "run" ]; then
  ./main
fi
