#include "../std_lib_facilities.h"

int main() {
	int n = -1;
	const vector<string> nums_names{"one", "ten", "hundred", "thousand"}; 
	vector<int> nums;
	const string txt = " and ";
	string out_str = "";
	const string s = "s";

	cout << "Enter a number:\n";
	cin >> n;

	if (n >= 10000) {
		cout << "Number too big (0, 10000)\n";
	} else if (n <= 0) {
		cout << "Number too small (0, 10000)\n";
	}

	while (n > 0) {
		nums.push_back(n % 10);
		n /= 10;
	}

	for (int i = nums.size() - 1; i >= 0; --i) {
		if (i != 0) out_str = txt + out_str;
		if (nums[i] == 0) continue;
		char tmp = nums[i] + '0';
		cout << tmp << " " << nums_names[i] << (nums[i] > 1 ? s : "") << (i != 0 ? txt : "");
		// out_str = tmp + "HERE" + nums_names[i] + (nums[i] > 1 ? s : " ") + out_str; 
	}

	cout <<  '\n';
	return 0;
}