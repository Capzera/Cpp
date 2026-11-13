#include <bits/stdc++.h>
using namespace std;
string s;
int t, k;
long long solve() {
	cin >> k >> s;
	long long ans = 0;
	int n = s.size();
	for (long long i = n - 1, j = 1; ~i; i--, j *= k) {
		int x = s[i] - 48;
		if (isalpha(s[i])) x -= 7;
		ans += x * j;
	}
	return ans;
}

int main() {
	cin >> t;
	while (t--) {
		cout << solve() << '\n';
	}
	return 0;
}
