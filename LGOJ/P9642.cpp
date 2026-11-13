#include <bits/stdc++.h>
using namespace std;
int t;
string s;
long long solve() {
	cin >> s;
	long long n = s.size();
	auto cal = [](long long x) -> long long {return x * (x + 1) / 2;};
	vector<long long> mp(10);
	for (auto& x : s) mp[x - '0']++;
	long long ans = cal(n) + 1;
	ans -= cal(mp[8]) + cal(mp[0]) + mp[6] * mp[9];
	if (mp[6] == n || mp[9] == n) return ans - 1;
	return ans;
}
int main() {
	cin >> t;
	while (t--) cout << solve() << endl;
	return 0;
}
