#include <bits/stdc++.h>
using namespace std;
int n, m, g[55];
struct stu {
	int id, make, rest[55];
} a[55];
bool cmp(stu x, stu y, int k) {
	if (x.rest[k] != y.rest[k]) return x.rest[k] < y.rest[k];
	if (x.make != y.make) return x.make > y.make;
	return x.id < y.id;
}
int main() {
	cin >> n >> m;
	for (int i = 1; i <= m; i++) cin >> g[i];
	for (int i = 1; i <= n; i++) {
		a[i].id = i, a[i].make = INT_MAX;
		for (int j = 1; j <= m; j++) {
			cin >> a[i].rest[j];
			a[i].make = min(a[i].make, a[i].rest[j] / g[j]);
		}
		for (int j = 1; j <= m; j++) {
			a[i].rest[j] -= a[i].make * g[j];
		}
	}
	for (int k = 1, f; k <= m; k++) {
		for (int i = 1; i < n; i++) {
			f = 1;
			for (int j = 1; j <= n - i; j++) {
				if (!cmp(a[j], a[j + 1], k)) {
					swap(a[j], a[j + 1]);
					f = 0;
				}
			}
			if (f) break;
		}
		for (int i = 1; i <= n; i++) cout << a[i].id << " ";
		cout << endl;
	}
	return 0;
}
/*
3 2
1 1
4 5
5 5
4 1
3 2 1



*/
