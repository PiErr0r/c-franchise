#include "../std_lib_facilities.h"

int main() {
	char c = 'a';
	for (char c = 'a'; c <= 'z'; ++c) {
		cout << c << '\t' << int(c) << '\t' << char(c - 'a' + 'A') << '\t' << int(c - 'a' + 'A') << '\n';
	}

	return 0;
}