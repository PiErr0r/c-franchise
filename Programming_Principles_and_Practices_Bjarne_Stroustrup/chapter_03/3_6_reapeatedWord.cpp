#include "../std_lib_facilities.h"

int main() {
	string previous = " ";
	string current;
	int wordNum = 0;

	while (cin >> current) {
		wordNum++;
		if (current == previous) {
			cout 	<< "repeated word :" << current 
						<< "\t\trepeated index: " << wordNum << "\n";
		}
		previous = current;
	}

	return 0;
}