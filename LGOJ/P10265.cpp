#include <bits/stdc++.h>
using namespace std;
int n, m, a[1001][1001], a1, a2;
int main() {
	cin >> n >> m;
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= n; j++) {
			cin >> a[i][j];
			if (i == m) a1 += a[i][j];
			if (j == m) a2 += a[i][j];
		}
	}
	cout << a1 << " " << a2 << " " << a1 + a2;
	return 0;
}
