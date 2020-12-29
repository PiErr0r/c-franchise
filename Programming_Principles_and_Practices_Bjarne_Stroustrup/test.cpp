//#include "std_lib_facilities.h"
#include <stdio.h>
#include<vector>

using namespace std;

void has_sum(vector<int>& a, int S) {
  int n = a.size();
  for (int mask = 0; mask < (1 << n); mask++) {
    long long sub_sum = 0;
    for (int i = 0; i < n; i++) {
      if (mask & (1 << i)){
        sub_sum += a[i];
      }
    }
    if (sub_sum == S) {
      puts("YES");
      return;
    }
  }
  puts("NO");
}

int main() {
  vector<int> a{1, 2, 3, 4, 5};
  for (int i = 0; i < 20; ++i){
    has_sum(a, i);
  }
}
