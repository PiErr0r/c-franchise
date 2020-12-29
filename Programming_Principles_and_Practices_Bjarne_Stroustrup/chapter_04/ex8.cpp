#include "../std_lib_facilities.h"

int main() {
	for (unsigned long long int i = 1, suma = 0, exp2 = 1; i <= 64; ++i, suma += exp2, exp2 *= 2) {
		if (suma > 1000000000) cout << "Billion\t" << i << '\t' << suma << '\n';
		else if (suma > 1000000) cout << "Million\t" << i << '\t' << suma << '\n';
		else if (suma > 1000) cout << "Thousand\t" << i << '\t' << suma << '\n';
	}

	return 0;
}