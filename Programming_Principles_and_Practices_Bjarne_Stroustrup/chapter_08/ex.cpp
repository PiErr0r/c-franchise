#include "../std_lib_facilities.h"

vector<int> reverse_new (const vector<int>& v) {
	vector<int> v_new;
	for (int i = v.size() - 1; i >= 0; --i) 
		v_new.push_back( v[i] );
	return v_new;
}

void reverse_swap (vector<int>& v) {
	int n = v.size();
	for (int i = 0; i < n / 2; ++i) {
		swap(v[i], v[n - i - 1]);
	}
}

void fibonacci(int first, int second, vector<int>& v, int n) {
	v.push_back(first);
	v.push_back(second);
	for (int i = 1; i < n - 1; ++i) {
		int tmp = v[i] + v[i - 1];
		if (tmp < 0)
			return;
		v.push_back(tmp);
	}
}

void print(const vector<int> v) {
	for (int i: v) 
		cout << i << " ";
	cout << '\n';
}

int main() {
	vector<int> v;
	fibonacci(1, 1, v, 20000);
	reverse_swap(v);
	print(v);
	print(reverse_new(v));

	return 0;
}