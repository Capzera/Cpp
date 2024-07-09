#include <bits/stdc++.h>
using namespace std;
int main() {
	long long n, ans = 0, cnt = 1;
	cin >> n;
	for (int i = 1; i <= n; i++) {
		cnt *= i;
		ans += cnt;
	}
	cout << ans << endl;
	return 0;
}
