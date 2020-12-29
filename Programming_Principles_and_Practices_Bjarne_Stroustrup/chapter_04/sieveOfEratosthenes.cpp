#include "../std_lib_facilities.h"

int main() {
	vector<int> primes;
	int max;

	cin >> max;

	for (int i = 2; i <= max; ++i) {
		primes.push_back(i);
	}

	int i = 0;
	while (i < primes.size()) {
		for (vector<int>::iterator it = primes.begin() + i + 1; it != primes.end(); ) {
			if (*it % primes[i] == 0) {
				primes.erase(it);
			} else {
				it++;
			}
		}
		i++;
	}

	for (int tmp : primes) {
		cout << tmp << '\n';
	}

	return 0;
}