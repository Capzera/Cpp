#include <bits/stdc++.h>
using namespace std;
long long n, m;
char a[1001][1001];
int main() {
	cin >> n >> m;
	for (int i = 1; i <= n; i++)
		for (int j = 1; j <= m; j++)
			cin >> a[i][j];
	for (int i = 1; i <= n; i++) {
		int ca = 0, cb = 0, f = 0;
		for (int j = 1; j <= m; j++) {
			ca += a[i][j] == 'A', cb += a[i][j] == 'B';
			if (a[i][j] == 'H') f = 1;
			else a[i][j] = '#';
		}
		if (f) {
			for (int j = 1; j <= ca; j++) a[i][j] = 'A';
			for (int j = m - cb + 1; j <= m; j++) a[i][j] = 'B';
			continue;
		}
		if (ca < cb) for (int j = 1; j <= cb; j++) a[i][j] = 'B';
		if (ca > cb) for (int j = m - ca + 1; j <= m; j++) a[i][j] = 'A';
	}
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= m; j++) {
			if (a[i][j] != 'H' && (a[i][j] == a[i + 1][j] || a[i][j] == a[i - 1][j])) cout << '#';
			else cout << a[i][j];
		}
		cout << endl;
	}
	
	return 0;
}
