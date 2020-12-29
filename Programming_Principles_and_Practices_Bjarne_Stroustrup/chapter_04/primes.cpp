#include "../std_lib_facilities.h"

int main() {
	vector<int> v;
	int i = 2, max = 0;
	cin >> max;
	while (i <= max) {
		bool isPrime = true;
		for (int pr: v) {
			if (pr > sqrt(i)) {
				break;
			}
			if (i % pr == 0) {
				isPrime = false;
				break;
			}
		}
		if (i % 1000000 == 0) {
			cout << i << '\n';
		} 
		if (isPrime) {
			v.push_back(i);
		}
		++i;
	}
 	for (int pr: v) {
 		cout << pr << '\n';
 	}
}