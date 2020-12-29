#include "../std_lib_facilities.h"

int main() {
	float x = 1.0 / 333;
	double y = 1.0 / 333;
	float sum_x {0};
	double sum_y {0};
	for (int i = 0; i < 333; ++i) {
		sum_x += x;
		sum_y += y;
	}

	cout << setprecision(15) << sum_x << '\t' << sum_y << '\n';
	return 0;
}