#include <bits/stdc++.h>
using namespace std;
long long x;
string solve() {
	cin >> x;
	if (x <= 10) return "R";
	if (x <= 20) return "L";
	return to_string(x);
}

int main() {
	int t;
	cin >> t;
	while (t--) {
		cout << solve() << '\n';
	}
	return 0;
}
