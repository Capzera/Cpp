#include <bits/stdc++.h>
using namespace std;
string s;
long long n, ans;
long long get(int l, int r) {
	long long cnt = 0;
	while (l >= 0 && r < n && s[l] == s[r]) {
		cnt++, l--, r++;
	}
	return cnt;
}
long long solve() {
	long long ret = 0;
	for (int i = 0; i < n; i++) {
		ret += get(i, i);
		ret += get(i, i + 1);
	}
	return ret;
}
int main() {
	cin >> s;
	n = s.size();
	for (int i = 0; i < n; i++) {
		char tmp = s[i];
		for (int j = 97; j <= 122; j++) {
			s[i] = j;
			long long sum = solve();
			ans = max(ans, sum);
			s[i] = tmp;
		}
	}
	cout << ans;
	return 0;
}
