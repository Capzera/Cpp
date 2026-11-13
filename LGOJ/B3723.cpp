#include <bits/stdc++.h>
using namespace std;
long long n, ans[2];
int main() {
	cin >> n;
	vector<long long> a(n);
	for (auto& x : a) cin >> x;
	for (int i = 0; i < n; i++) {
		long long money = ans[i % 2];
		long long mx = -1, mn = 0;
		for (int j = 0; j < a.size(); j++) {
			if ((mx == -1 && a[j] <= money) || (a[j] > a[mx] && a[j] <= money)) mx = j;
			if (a[j] < a[mn]) mn = j;
		}
		int kick = money < a[mn] ? mn : mx;
		ans[i % 2] += a[kick];
		a.erase(a.begin() + kick);
	}
	printf("%lld %lld", ans[0], ans[1]);
	return 0;
}

