#include <bits/stdc++.h>
using namespace std;
long long n, k;
string s;
int solve() {
	cin >> s;
	long long pro = 1;
	for (auto& x : s) {
		if (x == '0') return 1;
		if (pro <= k) pro *= x - '0';
	}
	return pro <= k;
}
int main() {
	cin >> n >> k;
	while (n--) cout << (solve() ? "kawaii\n" : "dame\n");
	return 0;
}
