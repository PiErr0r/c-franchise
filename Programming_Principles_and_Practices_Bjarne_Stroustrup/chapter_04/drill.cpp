#include "../std_lib_facilities.h"

int main() {
	double value, smallest = -1, largest = -1;
	double sum = 0, valuesNum = 0;
	string unit = "";
	vector <string> acceptedUnits = {"m", "cm", "in", "ft"};
	vector <double> values;
	

	cout << "Enter a value and a unit\t[m, cm, in, ft]\ne.g. 100cm\n";
	while (cin >> value >> unit) {

		if (value < 0) {
			cout << "value must be bigger than 0\n";
			continue;
		}

		bool isAccepted = false;
		for (string tmp : acceptedUnits) {
			if (tmp == unit) {
				isAccepted = true;
				if (unit == "m") {
					sum += value;
				} else if (unit == "cm") {
					sum += value / 100.0;
				} else if (unit == "ft") {
					sum += value * 0.3048;
				} else if (unit == "in") {
					sum += value * 2.54 / 100;
				}
				values.push_back(value);
				break;
			}
		}
		if (!isAccepted) {
			cout << "Invalid unit!\n";
			continue;
		}

		if (smallest == largest && smallest == -1) {
			smallest = value;
			largest = value;
		} else {
			if (value < smallest) {
				smallest = value;
			}
			if (value > largest) {
				largest = value;
			}
		}	
	}

	cout << "the smallest so far: " << smallest << '\n'
			 << "the largest so far: " << largest << '\n'
			 << "the sum: " << sum << " meters\n"
			 << "number of values entered: " << values.size() << '\n';
	sort(values);
	for (double val : values) {
		cout << val << '\n';
	}


	return 0;
}