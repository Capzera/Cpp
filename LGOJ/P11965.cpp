#include <bits/stdc++.h>
using namespace std;
string s;
int n, mp[67108864], cnt[26];
long long ans;
int main() {
	cin >> n >> s;
	mp[0] = 1;
	for (int i = 0, mask = 0; i < n; i++) {
		int ch = s[i] - 'a';
		cnt[ch] ^= 1;
		mask ^= 1 << ch;
		ans += mp[mask];
		mp[mask]++;
	}
	cout << ans;
	return 0;
}
