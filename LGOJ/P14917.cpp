#include <bits/stdc++.h>
using namespace std;
long long n, a[100001];
bool check(int x) {
	for (int i = 1, lst = 0; i <= n; i++) {
		if (a[i] <= x) continue;
		if (lst && a[i] != lst) return 0;
		lst = lst == a[i] ? 0 : a[i];
	}
	return 1;
}
int main() {
	cin >> n;
	for (int i = 1; i <= n; i++) cin >> a[i];
	int l = 1, r = 100001, ans = 0;
	while (l < r) {
		int mid = l + (r - l >> 1);
		if (check(mid)) r = ans = mid;
		else l = mid + 1;
	}
	cout << ans;
	return 0;
}
