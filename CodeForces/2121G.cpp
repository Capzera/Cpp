#include <bits/stdc++.h>
using namespace std;
string s;
long long n;
void solve() {
	cin >> n >> s;
	vector<long long> sum(n + 1);
	long long ans = 0;
	for (int i = 1; i <= n; i++) {
		sum[i] = sum[i - 1] + (s[i - 1] == '1' ? 1 : -1);
	}
	sort(sum.begin(), sum.end());
	for (int i = 1; i <= n; i++) {
		ans += (sum[i] - sum[i - 1]) * i * (n - i + 1);
	}
	cout << (ans + n * (n + 1) / 2 * (n + 2) / 3) / 2 << endl;
}
int main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	cout.tie(nullptr)->sync_with_stdio(0);
	int t;
	cin >> t;
	while (t--) solve();
	return 0;
}
/*

*/
