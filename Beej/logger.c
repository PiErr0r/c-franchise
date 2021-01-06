#include <stdio.h>

void err(char *msg) {
  fprintf(stderr, "[ERROR]\t%s\n", msg);
}

void info(char* msg) {
  fprintf(stdout, "[INFO]\t%s\n", msg);
}

