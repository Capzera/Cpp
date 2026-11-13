#include <bits/stdc++.h>
using namespace std;
string s;
long long n = 4617, m, f = 1;
int main() {
	cin >> m;
	while (n--) {
		cin >> s;
		int num = 0;
		for (auto& x : s) {
			num = num * 10 + (x - 'A' - (x > 'Q')) / 3 + 2;
		}
		if (num == m) {
			cout << s << '\n';
			f = 0;
		}
	}
	if (f) return puts("NONE"), 0;
}
/*

*/
