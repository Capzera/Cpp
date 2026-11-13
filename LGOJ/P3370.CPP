#include <bits/stdc++.h>
using namespace std;
const long long base = 131, mod = 998244353;
long long n, ans = 1;
string s;
int main() {
	cin >> n;
	vector<long long> hash;
	while (n--) {
		cin >> s;
		long long key = 0;
		for (auto& x : s) {
			key = (key * base + x) % mod;
		}
		hash.push_back(key);
	}
	sort(hash.begin(), hash.end());
	for (int i = 1; i < hash.size(); i++) {
		ans += hash[i] != hash[i - 1];
	}
	cout << ans;
	return 0;
}
