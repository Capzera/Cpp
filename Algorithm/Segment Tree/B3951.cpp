#include <bits/stdc++.h>
using namespace std;
int n, m, x, a[2001], v[2001], mx, ans;
int main() {
	cin >> n;
	for (int i = 0; i < n; i++) cin >> a[i];
	cin >> m;
	for (int i = 0; i < m; i++) {
		cin >> x;
		v[i] = a[x];
		int ans = 0;
		for (int j = 0; j < m; j++) {
			if (v[j] > v[i]) {
				swap(v[i], v[j]);
				ans++;
			}
		}
		cout << ans << endl;
	}
	return 0;
}
/*
4
20 20 20 10
4
0 1 2 3

5
170 165 168 160 175
4
0 3 2 1
160 170 168


*/
