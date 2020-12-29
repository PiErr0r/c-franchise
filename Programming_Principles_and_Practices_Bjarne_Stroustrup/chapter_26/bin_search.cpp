#include "../std_lib_facilities.h"
#define ll long long

int b_search(vector<string>& v, string el) {
	unsigned long int start {0};
	unsigned long int end {v.size()};
	int mid = -1;
	while (end > start) {
		mid = (start + end) / 2;
		if (el < v[mid])
			end = mid;
		else if (el > v[mid])
			start = mid + 1;
		else 
			break;
	}
	return (v[mid] == el ? mid : -1);
}

int main() {
	vector<string> v {"Bohr", "Darwin", "Einstein", "Lavoisier", "Newton", "Turing"};
	cout << b_search(v, "") << '\n';
	cout << b_search(v, "bohr") << '\n';
	cout << b_search(v, "Bohr") << '\n';
	cout << b_search(v, "Turing") << '\n';

	return 0;
}