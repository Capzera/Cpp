#include <bits/stdc++.h>
using namespace std;
long long n, p, t;
int main() {
	cin >> n >> p;
	string ans(n, '0');
	ans[0]++;
	if (p > 9 * n) return puts(ans.c_str()), 0;
	t = --p / 9, p %= 9;
	ans.replace(n - t, t, t, '9');
	ans[n - t - 1] += p;
	cout << ans;
	return 0;
}
/*
n <= 1e6, p <= 1e9
*/
