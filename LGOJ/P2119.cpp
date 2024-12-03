#include <bits/stdc++.h>
using namespace std;
long long n, m, a[40010], ans[40010][4];
unordered_map<int, int> mp;
int main() {
	cin >> m >> n;
	for (int i = 1; i <= n; i++) {
		cin >> a[i];
	}
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= n; j++) {
			if (j == i) continue;
			for (int k = 1; k <= n; k++) {
				if (k == i || k == j) continue;
				for (int l = 1; l <= n; l++) {
					if (l == i || l == j || l == k) continue;
					if (a[i] < a[j] && a[j] < a[k] && a[k] < a[l]) {
						if (a[j] - a[i] == 2 * (a[l] - a[k]) && 3 * (a[j] - a[i]) < a[k] - a[j]) {
							ans[i][0]++, ans[j][1]++, ans[k][2]++, ans[l][3]++;
						}
					}
				}
			}
		}
	}
	for (int i = 1; i <= n; i+a+) {
		for (int j = 0; j < 4; j++) {
			cout << ans[i][j] << " ";
		}
		cout << endl;
	}
	return 0;
}
/*
b - a = 2d - 2c
3(b - a) < c - b
6d - 6c < c - b
b < 6(c - d) + c;

1 24 7 28 5 29 26 24
1 5 7 24 24 26 28 29
b > (d - c) * 5 + d
b - a = 2d - 2c
a = b - 2d - 2c
1 <= 9t + k <= n
1 <= t <= (n - 1) / 9
9 <= 9t < n 

C = D - t;

*/
