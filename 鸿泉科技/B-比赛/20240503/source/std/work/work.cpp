#include <bits/stdc++.h>
using namespace std;
int main() {
	freopen("work.in", "r", stdin);
	freopen("work.out", "w", stdout);
	int n, x, ans = 0, i = 0;
	cin >> n >> x;
	vector<int> a(n);
	long long sum = 0;
	for (auto& y : a) {
		cin >> y;
		sum += y - x;
	}
	sort(a.begin(), a.end());
	for (; i < n; i++) {
		if (sum >= 0) break;
		sum -= a[i] - x;
	}
	cout << n - i << endl;	
	return 0;
}
