#include <bits/stdc++.h>
using namespace std;
long long n, mx;
int main() {
	cin >> n;
	vector<long long> a(n + 2);
	for (int i = 1; i <= n; i++) {
		cin >> a[i];
		mx = max(mx, a[i]);
	}
	vector<long long> b(2 * mx), sum(2 * mx);
	for (int i = 1; i <= n; i++) {
		b[a[i]] += (a[i] > a[i - 1]) - (a[i + 1] > a[i]);
	}
	for (int i = 1; i < 2 * mx; i++) sum[i] = sum[i - 1] + b[i];
	for (int i = 1; i <= mx; i++) {
		long long ans = 0;
		for (int j = 1; j <= mx; j += i) {
			ans += ((j + i - 1) / i) * (sum[j + i - 1] - sum[j - 1]);
		}
		cout << ans << " ";
	}
	cout<<endl;
	for (auto& x : b) cout <<x << " ";cout << endl;
	for (auto& x : sum) cout << x << " ";cout <<endl;
	return 0;
}
