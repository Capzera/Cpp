#include <bits/stdc++.h>
using namespace std;
int n, cnt[26], mx, mn = 101;
string s;
bool isPrime(int x) {
	if (x < 2) return 0;
	if (x == 2 || x == 3) return 1;
	if (x % 6 != 1 && x % 6 != 5) return 0;
	int n = sqrt(x);
	for (int i = 5; i <= n; i += 6) {
		if (x % i == 0 || x % (i + 2) == 0) return 0;
	}
	return 1;
}
int main() {
	cin >> s;
	n = s.size();
	for (int i = 0; i < n; i++) {
		cnt[s[i] - 'a']++;
	}
	for (int i = 0; i < 26; i++) {
		if (!cnt[i]) continue;
		mx = max(mx, cnt[i]);
		mn = min(mn, cnt[i]);
	}
	mx -= mn;
	if (isPrime(mx)) {
		cout << "Lucky Word" << endl << mx;
	} else {
		cout << "No Answer" << endl << 0;
	}
	return 0;
}
