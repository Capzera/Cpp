#include <bits/stdc++.h>
using namespace std;
int main() {
	int n;
	cin >> n;
	vector<int> a(n);
	for (auto& x : a) cin >> x;
	sort(a.begin(), a.end());
	int mx = a[0];
	for (int i = 1; i < n; i++) {
		mx = max(mx, a[i] - a[i - 1]);
	}
	cout << mx << endl;
	return 0;
}
