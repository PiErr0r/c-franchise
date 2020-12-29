#include "../std_lib_facilities.h"

int main() {
	int* p = new int[10];
	for (int j = 0; j < 10; ++j) 
		cout << (p+j) << "\n";
	return 0;
}