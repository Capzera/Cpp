#include <bits/stdc++.h>
using namespace std;
long long n, a[1001][4], b[1001][4];
int main() {
	cin >> n;
	for (int i = 0; i < n; i++)
		for (int j = 0; j < 4; j++)
			cin >> a[i][j];
	for (int i = 0; i < n; i++)
		for (int j = 0; j < 4; j++)
			cin >> b[i][j];
	for (int t = 0; t < n; t++) {
		int ans = 0;
		for (int i = 0; i < n; i++) {
			int f = 0, ca = 0, cb = 0;
			for (int j = 0; j < 4; j++) {
				if (!a[(i + t) % n][j] && b[i][j]) f = 1;
				if (a[(i + t) % n][j]) ca++;
				if (b[i][j]) cb++;
			}
			if (f) continue;
			if (ca == cb) ans += 6;
			if (cb && ca > cb) ans += 3;
		}
		cout << ans << " ";
	}
	return 0;
}
/*
0 0 1 0
0 1 0 0
0 1 1 0  0

0 1 0 1  
0 1 1 0
0 0 1 1  a

*/
