#include "../std_lib_facilities.h"

int main() {
	string a, b, c;

	cin >> a >> b >> c;

	(a < b && a < c) ? (b < c ? cout << a << " " << b << " " << c : cout << a << " " << c << " " << b) : (b < a && b < c) ? (a < c ? cout << b << " " << a << " " << c : cout << b << " " << c << " " << a) : a < b ? cout << c << " " << a << " " << b : cout << c << " " << b << " " << a; 

	cout << "\n";
	return 0;
}