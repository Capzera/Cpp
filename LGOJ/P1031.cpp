#include <bits/stdc++.h>
using namespace std;
int main() {
	int n, avg = 0, ans = 0;
	cin >> n;
	vector<int> a(n);
	for (auto& x : a) {
		cin >> x;
		avg += x;
	}
	avg /= n;
	for (int i = 0; i < n - 1; i++) {
		if (a[i] == avg) continue;
		a[i + 1] += a[i] - avg;
		ans++;
	}
	cout << ans << endl;
	return 0;
}
