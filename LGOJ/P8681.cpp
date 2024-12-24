#include <bits/stdc++.h>
using namespace std;
long long n, x, mx = INT_MIN, ans = 1, cnt, depth = 1;
int main() {
	cin >> n;
	for (int i = 1, j = 2; i <= n; i++) {
		cin >> x;
		if (i == n || i == j) {
			j <<= 1;
			if (cnt > mx) {
				mx = cnt;
				ans = depth;
			}
			depth ++;
			cnt = x;
		} else cnt += x;
	}
	cout << ans;
	return 0;
}
