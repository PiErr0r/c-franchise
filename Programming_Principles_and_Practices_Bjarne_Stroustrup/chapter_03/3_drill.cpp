#include "../std_lib_facilities.h"

int main() {
	string firstName, friendName, outString;
	char friendSex {0};
	int age, tmp;
	string ageString = "";

	cout << "Enter the name of the person you want to write to:\n";

	cin >> firstName;
	outString	= "\tDear " + firstName + ",\n" + "How are you? I am fine. I miss you.\n";
	cout << outString;


	cout << "Enter a name of another friend:\n";
	cin >> friendName;
	outString += "Have you seen " + friendName + " lately?\n";
	cout << outString;

	while (friendSex != 'f' && friendSex != 'm') {
		cout << "Enter the sex of the friend [m/f]:\n";
		cin >> friendSex;
	}

	outString += "If you see " + friendName + " please ask " + (friendSex == 'f' ? "her" : "him") + " to call me\n";
	cout << outString;

	cout << "What is recipients age?\n";
	cin >> age;

	if (age < 0 || age > 110) {
		simple_error("you're kidding");
	}
	tmp = age;
	while (tmp > 0) {
		char x = tmp % 10 + '0';
		ageString = x + ageString;
		tmp /= 10;
	}


	outString += "I heard you just had a birthday and you are " + ageString + " years old!\n";
	cout << outString;

	if (age < 12) {
		tmp = ageString.back();
		tmp++;
		ageString[ageString.length() - 1] = tmp;

		outString += "Next year you will be " + ageString + "\n";
	} else if (age == 17) {
		outString += "Next year you will be able to vote.\n";
	} else if (age > 70) {
		outString += "I hope you are enjoying retirement.\n";
	}

	outString += "\nYours sincerely\n\n";
	outString += "Piero\n";

	cout << outString;

	return 0;
}