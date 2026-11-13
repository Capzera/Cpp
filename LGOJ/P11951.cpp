#include <bits/stdc++.h>
using namespace std;
const int N = 8010;
long long n, ans, a[N];
int main() {
	cin >> n;
	for (int i = 1; i <= n; i++) cin >> a[i];
	sort(a + 1, a + n + 1);
	for (int k = 3; k <= n; k++) { // 枚举最长边为a[k]
		int l = 1, r = k - 1;
		while (l < r) {
			if (a[l] + a[r] > a[k]) {
				ans += r - l;
				r--;
			} else l++;
		}
	}
	/*for (int i = 1; i < n - 1; i++) { // 边1
		for (int j = i + 1; j < n; j++) { // 边2
			int k = lower_bound(a + 1, a + n + 1, a[i] + a[j]) - a;
			ans += k - 1 - j;
		}
	}*/
	cout << ans;
	return 0;
}
