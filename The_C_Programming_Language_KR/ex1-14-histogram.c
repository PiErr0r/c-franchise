#include <stdio.h>
#define CHARS 128
#define STEP 1

/*
 * Best way to test is using this program as input
 */
int main() {

  int n[CHARS] = {0}, max = 0, i, j;
  char c;
  while ((c = getchar()) != EOF) {
    if (c == ' ' || c == '\n' || c == '\t') continue;
    n[c]++;
    if (n[c] > max) max = n[c];
  }

  for (i = 0; i < max; ++i) {
    for (j = 0; j < CHARS; ++j) {
      if (!n[j] || (char)j == ' ' || (char)j == '\n' || (char)j == '\t') continue;
      if (n[j] >= max - i) {
        printf("   #");
      } else {
        printf("    ");
      }

    }
    printf("\n");
  }
  for (i = 0; i < CHARS; ++i) {
    if (n[i] && (char)i != ' ' && (char)i != '\n' && (char)i != '\t') {
      printf("%4c", i);
    }
  }
  printf("\n");

  return 0;
}
