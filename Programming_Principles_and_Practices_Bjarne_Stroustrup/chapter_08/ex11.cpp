#include "../std_lib_facilities.h"

struct analysis {
	int min;
	int max;
	double mean;
	double median;
};

analysis analyze(vector<int>& v) {
	analysis ret;
	int n = v.size();
	sort(v);

	ret.min = v[0];
	ret.max = v[n - 1];
	if (n % 2 == 0) 
		ret.mean = (1.0 * v[n / 2] + v[n / 2 - 1]) / 2;
	else
		ret.mean = v[n / 2];

	int sum = 0;
	for (int i : v) {
		sum += i;
	}
	ret.median = 1.0 * sum / n;

	return ret;
}

int main() {
	vector<int> v {1, 2, 3, 4, 5, 6, 8};
	analysis asd;
	asd = analyze(v);
	cout << asd.min << " " << asd.max << " " << asd.mean << " " << asd.median << '\n';
	return 0;
}