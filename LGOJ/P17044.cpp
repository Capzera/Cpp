#include <bits/stdc++.h>
#define pii pair<long long, unsigned long long>
using namespace std;
const int N = 1e6 + 10;
long long n, a[N];
int check(long long x) {
	unsigned long long cur = x;
	stack<pii> stk;
	for (int i = 1; i <= n * 2; i++) {
		if (a[i] > cur) {
			stk.push({a[i], cur + a[i] - x});
			cur = x;
			continue;
		}
		cur += a[i];
		while (stk.size() && stk.top().first <= cur) {
			cur += stk.top().second; stk.pop();
		}
	}
	return stk.empty();
}
int main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	cin >> n;
	for (int i = 1; i <= n; i++) {
		cin >> a[i];
		a[i + n] = a[i];
	}
	
	long long l = 0, r = 1e13 + 1, ans = 0;
	while (l < r) {
		long long mid = l + (r - l >> 1);
		if (check(mid)) ans = r = mid;
		else l = mid + 1;
	}
	cout << ans;
	return 0;
}
