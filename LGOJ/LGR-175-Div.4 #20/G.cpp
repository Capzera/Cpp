#include <bits/stdc++.h>
using namespace std;
long long n, q, c1, c2;
string s, t;
int main() {
	cin >> n >> q >> s >> t;
	for (auto& x : t) {
		if (x == '1') c1 ^= 1;
		if (x == '2') c2 ^= 1;
	}
	if (c2) {
		for (auto& x : s) {
			x = ((x - 48) ^ 1) + 48;
		}
	}
	if (c1) reverse(s.begin(), s.end());
	cout << s;
	return 0;
}
