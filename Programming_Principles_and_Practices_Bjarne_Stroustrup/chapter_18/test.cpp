#include "../std_lib_facilities.h"

string ccat(const string& s1, const string& s2) {
	return s1 + '.' + s2;
}

int main() {
	string a = "test";
	string b = "the_dot";
	vector<int> v {0, 1};
	cout << ccat(a, b) << '\n';

	// int* p1 = 

	cout << (&v[1] - &v[0]) << '\n';

	return 0;
}