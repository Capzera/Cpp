#include <bits/stdc++.h>
using namespace std;
long long n, m, r1, c1, r2, c2;
char x;
int main() {
	cin >> n >> m >> r1 >> r2 >> c1 >> c2;
	for (int i = 1; i <= n; i++) {
		int f = 0;
		for (int j = 1 ; j <= m; j++) {
			cin >> x;
			if (i < r1 || i > r2 || j < c1 || j > c2) continue;
			cout << x;
			f = 1;
		}
		if (f) cout << endl;
	}
	return 0;
}
