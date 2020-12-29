#include "../std_lib_facilities.h"

int main() {
	int number = 0;

	while (number < 1 || number > 100) {
		cout << "Enter a number [1, 100]\n";
		cin >> number;
	}

	cout << "I will now guess your number.\nAnswer with y for yes or n for no!\n";

	int i = 0;
	int guess = 50;
	int top = 101, bot = 0;
	char ans = ' ';
	while (i < 7) {
		cout << "Is you number greater than " << guess << "?\n";
		cin >> ans;
		switch (ans) {
			case 'y':
				bot = guess;
				break;
			case 'n':
				top = guess + 1;
				break;
			default:
				cout << "That is not a valid answer!!\n";
				break;
		}
		guess = bot + (top - bot) / 2;

		if (top - bot <= 2) {
			cout << "Your number is " << guess << '\n';
			break;
		}
		++i;
	}
	cout << i << '\t' << "END\n";
	return 0;
}