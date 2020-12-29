#include "../std_lib_facilities.h"

int main() {
	ifstream ist {"image_1.jpg"};
	vector<string> v;
	for (string tmp; ist >> tmp;) {
		v.push_back(tmp);
	}

	for (string tmp : v) {
		cout << ">\t" << tmp << '\n';
	}

	return 0;
}