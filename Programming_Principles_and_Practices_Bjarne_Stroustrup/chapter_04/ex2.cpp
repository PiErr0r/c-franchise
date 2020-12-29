#include "../std_lib_facilities.h"

int main() {
	vector<int> arr;

	for (int temp; cin >> temp;) {
		arr.push_back(temp);
	}

	int size = arr.size();
	sort(arr);

	if (size % 2 == 1) {
		cout << arr[ size / 2 ] << '\n';
	} else {
		cout << ( 1.0 * arr[size / 2] + arr[size / 2 - 1] ) / 2 << '\n';
	}

	return 0;
}