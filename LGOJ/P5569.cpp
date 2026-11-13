#include<bits/stdc++.h>
using namespace std;
long long n, w, ans;
int main() {
	cin >> n;
	vector<int> a(n);
	for (int i = 0; i < n; ++i) cin >> a[i];
	for (int i = 0; i < n - 1; i++) {
		int k = a.size() - 2;
		for (int i = 0; i < k; i++) {
			if (a[i] > a[i + 2]) continue;
			k = i;
			break;
		}
		int cnt = a[k] + a[k + 1], j = -1;
		a.erase(a.begin() + k), a.erase(a.begin() + k);
		for (int i = k - 1; i >= 0; i--) {
			if (a[i] <= cnt) continue;
			j = i;
			break;
		}
		a.insert(a.begin() + j + 1, cnt);
		ans += cnt;
	}
	cout << ans;
	return 0;
}
