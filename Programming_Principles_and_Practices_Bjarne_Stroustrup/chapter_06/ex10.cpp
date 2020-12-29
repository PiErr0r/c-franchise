#include "../std_lib_facilities.h"

int fact(int n) {
	if (n == 0) return 1;
	else if (n < 0) error("Numbers must be positive");
	int ret = 1;
	for (int i = 2; i <= n; ++i) ret *= i;
	return ret;
}

int permutations(int n, int k) {
	if (k > n) error("Number of possibilities must be bigger than number of choices - permutations");
	return fact(n) * 1.0 / fact(k);
}

int combinations(int n, int k) {
	if (k > n) error("Number of possibilities must be bigger than number of choices - combinations");
	return permutations(n, k) / fact(k);
}

int main() {
	int n = -1, k = -1;
	char mode = '\0';
	cout << "Enter number of possibilities, number of choices and a mode\nc for combinations, p for permutations\neg. 10 3 p\n";

	cin >> n >> k >> mode;

	try {
		switch(mode) {
			case 'p':
				cout << permutations(n, k);
				break;
			case 'c':
				cout << combinations(n, k);
				break;
			default:
				error("unknown mode");
		}
		cout << '\n';
	} catch (exception& e) {
		cerr << "error: " << e.what() << '\n';
		return 1;
	} catch(...) {
		cerr << "Uncaught exception\n";
		return 2;
	}

	return 0;
}