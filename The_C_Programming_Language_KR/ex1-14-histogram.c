#include <stdio.h>
#define CHARS 128
#define STEP 1
#define MAX_H 15

/*
 * Best way to test is using this program as input
 */
int main() {

  unsigned long long int n[CHARS] = {0}, max = 0, i, j;
  double result[CHARS];
  double res;
  char c;
  while ((c = getchar()) != EOF) {
    if (c == ' ' || c == '\n' || c == '\t') continue;
    n[c]++;
    if (n[c] > max) max = n[c];
  }

  res = (double)max / (double)MAX_H;
  printf("Each # is approximately %.5lf occurrences.\n", res);

  for (i = 0; i < CHARS; ++i) {
    result[i] = (double)n[i] / res;
  }

  for (i = 0; i <= MAX_H; ++i) {
    for (j = 0; j < CHARS; ++j) {
      if (result[j] == 0 || (char)j == ' ' || (char)j == '\n' || (char)j == '\t') continue;
      if (result[j] >= MAX_H - i) {
        printf(" #");
      } else {
        printf("  ");
      }

    }
    printf("\n");
  }
  for (i = 0; i < CHARS; ++i) {
    if (n[i] && (char)i != ' ' && (char)i != '\n' && (char)i != '\t') {
      printf("%2c", i);
    }
  }
  printf("\n");

  return 0;
}
