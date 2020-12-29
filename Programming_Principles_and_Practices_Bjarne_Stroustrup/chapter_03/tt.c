#include "../std_lib_facilities.h"

int main()
{
	// cout << "Please enter a floating-point value: ";
	cout << "Please enter two integer values: n and a";
	int n, a;
	cin >> n >> a;
	cout << "n == " << n
		<< "\nn+1 == " << n+1
		<< "\nthree times n == " << 3*n
		<< "\ntwice n == " << n+n
		<< "\nn squared == " << n*n
		<< "\nhalf of n == " << n/2
		<< "\nsquare root of n == " << sqrt(n)
		<< "\na/b * b + a%b == a  " << a/n * n + a%n << " " << a/n << " "  << a/n * n << " " << a % n
		<< "\n"; // another name for newline (“end of line”) in output
}