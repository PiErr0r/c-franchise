#include "../std_lib_facilities.h"

int main() {
	vector<int> v;
	v.push_back(0);
	v.push_back(0);
	v.push_back(0);
	v.push_back(0);
	v.pop_back();
	v.pop_back();
	v.pop_back();
	cout << v.capacity() << " " << v.size() << '\n';
	return 0;
}