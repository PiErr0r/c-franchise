#include "../std_lib_facilities.h"

int main() {
	ifstream ist {"1_input"};
	if (!ist) {
		error("cant read file");
	}

	string in_data;
	ist >> in_data;
	int sum = 0, dataLen = in_data.size();

	for (int i = 0; i < dataLen; ++i) {
		if (in_data[i] == in_data[(i + dataLen / 2) % dataLen]) 
			sum += (in_data[i] - '0');
	}
	cout << sum << '\n';

	return 0;
}