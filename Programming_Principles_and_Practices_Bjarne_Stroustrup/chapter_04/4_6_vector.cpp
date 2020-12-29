#include "../std_lib_facilities.h"

int main() {
	vector<string> badWords = {"fuck", "ass", "donkey"};

	for (string tmp; cin >> tmp;) {
		bool isBad = false;
		for (string bad : badWords) {
			if (tmp == bad) {
				isBad = true;
				break;
			}
		}
		if (isBad) cout << "BLEEP\n";
		else cout << tmp << '\n';
	}

	return 0;
}