#include "../std_lib_facilities.h"

int main() {
	int val1 = 0, val2 = 0;
	double res = 0;

	cout << "Enter two values:\n";
	cin >> val1 >> val2;

	res = val1 / val2 * 1.0 ;
	cout << res;
	cout << "\n";
	return 0;
}