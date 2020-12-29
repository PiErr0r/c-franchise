#include "../std_lib_facilities.h"

bool p (int a) {
	if (a != 5) {
		return true;
	} else {
		return false;
	}

}

int w_sum(const pair<char, double>& a, const pair<char, double>& b) {
	return a.second * b.second;
}

int max (const vector<int>& a) {
	if (a.size() == 0) {
		return -1;
	}
	int max = a[0];
	for (const auto& p : a) {
		if (p > max) {
			max = p;
		}
	}
	return max;
}

int main() {
	vector<int> v {5, 2, 3, 4, 7, 3, 6, 7, 8};
	unordered_map<char, int> prices;
	unordered_map<char, int> weight;
	for (char i = 'a'; i <= 'z'; ++i) 
		prices[i] = i;
	for (char i = 'Z'; i >= 'A'; --i)
		weight[i] = 'A' + ('Z' - i);
	int tmp = inner_product(prices.begin(), prices.end(), weight.begin(), 0, plus<double>(), w_sum);
	cout << accumulate(v.begin(), v.end(), 1) << '\n';
	cout << tmp << '\t' << max(v) << '\n';
	for(auto& p : prices) {
		cout << p.first << '\t' << p.second << '\t';
 	}
	return 0;
}