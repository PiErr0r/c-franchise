#include "../std_lib_facilities.h"

int main() {
	int mode = -1, cnt = 1, max = 0;
	vector<int> posInt = {1,2,3, 2,1,4,5,2,1,3,4, 1, 2,3, 1,3, 2,1,5 ,4,5, 2,6,4,3, 4,5, 7,5,3,6, 7, 4,3, 5,2 ,5,3,4,2, 5,2,3, 5,4, 2,3,5, 2,3,4 ,5,2,3, 6,2,4, 5,6,2 ,4 ,6,2,3 ,4, 5,2,3,4, 5,2 ,4,6, 4,5, 6,5, 2,6,3, 2,6,4 ,6, 4,3,6, 4,5 ,2,6,5 ,6,2,3,5 ,4 ,5,2 ,4,5,6 ,5,4,2,6,4,2,2,6,5,6,2,4,5,6,2,6,5,6,4,6,3,4,6};

	sort(posInt);
	for (int i = 0; i < posInt.size(); ++i) {
		if (mode == -1) {
			mode = posInt[i];
			continue;
		}
		if (posInt[i] == posInt[i-1]) {
			cnt += 1;
		} else {
			if (cnt > max) {
				max = cnt;
				mode = posInt[i - 1];
			}
			cnt = 1;
		}
	}
	for (int i : posInt) {
		cout << i << " ";
	}
	cout << '\n' << mode << '\n';
	return 0;
}