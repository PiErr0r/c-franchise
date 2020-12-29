#include "../std_lib_facilities.h"

int square(int x) {
	int res = 0;
	for (int i = 0; i < x; ++i) {
		res += x;
	}
	return res;
}

int main() {
	for (int i = 0; i < 100; ++i) {
		cout << i << '\t' << square(i) << '\n';
	}
	return 0;
}