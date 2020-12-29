#include "../std_lib_facilities.h"

bool solution(std::string const &str, std::string const &ending) {
		auto p = ending.end() - 1;
		auto q = str.end() - 1;
    while (p >= ending.begin()) {
      if (*p != *q) {
        return false;
      }
      --p; --q;
    }
    return true;
}

int main() {

	const string s = "asd";
	const string c = "d";
	cout << solution(s, c);
	return 0;
}