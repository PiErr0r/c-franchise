#include "../std_lib_facilities.h"

int main() {
	constexpr double yen2Dollar = 0.0092;
	constexpr double eur2Dollar = 1.08;
	constexpr double kro2Dollar = 0.094;

	int value;
	char currency;

	cout << "Enter the value and currency to calculate amount in dollars: e.g. 200y \n";
	cout << "[yen = y] [euro = e] [kro = k]\n";

	cin >> value >> currency;

	switch(currency) {
		case ('y'):
			cout << "$ " << value * yen2Dollar << "\n"; 
			break;
		case('e'):
			cout << "$ " << value * eur2Dollar << "\n";
			break;
		case('k'):
			cout << "$ " << value * kro2Dollar << "\n";
			break;
		default:
			cout << "Unknown currency\n";
	}

	return 0;
}