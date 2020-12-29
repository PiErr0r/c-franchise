#include "../std_lib_facilities.h"

int main() {
	vector<double> distances;
	double sum = 0;

	for (double tmp; cin >> tmp;) {
		distances.push_back(tmp);
	}	

	double min = distances[0], max = distances[0];
	for (vector<double>::iterator it = distances.begin(); it != distances.end(); ++it) {
		sum += *it;
		min = *it < min ? *it : min;
		max = *it > max ? *it : max;
	}

	cout 	<< "SUM:\t" << sum << '\n'
				<< "MEAN:\t" << sum / distances.size() << '\n'
				<< "MIN\t" << min << '\n'
				<< "MAX\t" << max << '\n';
	
	return 0;
}