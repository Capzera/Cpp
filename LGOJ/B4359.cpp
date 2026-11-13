#include <bits/stdc++.h>
using namespace std;
long long n, x, ans;
int main() {
	cin >> n >> ans;
	for (long long i = 1, cnt = ans; i < n; i++) {
		cin >> x;
		cnt = max(cnt + 1, x);
		ans += cnt;
	}
	cout << ans;
	return 0;
}
