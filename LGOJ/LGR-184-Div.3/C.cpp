#include <bits/stdc++.h>
using namespace std;
void solve() {
	int n;
	cin >> n;
	vector<int> a(n);
	for (int i = 0; i < n; i++) {
		cin >> a[i];
	}
	if (n == 2) {
		cout << "Yes" <<endl;
		return;
	}
	sort(a.begin(), a.end(), greater<int>());
	int t = __gcd(a[0], a[1]);
	if (a.back() == t && a[2] == t) {
		cout << "Yes" << endl;
	} else cout << "No" << endl;
	return;
}
int main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	int t;
	cin >> t;
 	while (t--) {
		solve();
	}
	return 0;
}
/*

*/
