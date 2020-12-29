#include "../std_lib_facilities.h"

int main() {
	cout << "Enter your first name and age:\n";
	string name = "???";
	int age = -1;
	cin >> name;
	cin >> age;
	cout << "Hello " << name << "!\n" << "age:" << age << "\n" ;
	return 0;
}