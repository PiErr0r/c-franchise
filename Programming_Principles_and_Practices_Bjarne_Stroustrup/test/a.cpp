#include <iostream>

using namespace std;

int rangeBitwiseAnd(int a, int b) {
  int answer = 0;
  for (int bit = 30; bit >= 0; --bit) {
    int t = 1 << bit;
    //(t & a != t & b) ? {break;} : answer |= t;
    if ((t & a) != (t & b)) break;
    else answer |= t & a;
  }
  return answer;
}

int main() {
  int a = 5, b = 7;

  cout << rangeBitwiseAnd(a, b) << endl;
  cout << rangeBitwiseAnd(17, 31) << endl;
  cout << rangeBitwiseAnd(1000, 3000) << endl;
  return 0;
}
