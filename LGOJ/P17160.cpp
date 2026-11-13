#include <bits/stdc++.h>
#define int long long
using namespace std;
const int N = 1e3 + 10;
int n, m, a[N][N];
char op;
void calc() {
	int ans[n][m];
	for (int i = 1; i < n; i++)
		for (int j = 1; j < m; j++)
			ans[i][j] = a[i][j] * a[i + 1][j + 1] - a[i + 1][j] * a[i][j + 1];
	for (int i = 1; i < n; i++)
		for (int j = 1; j < m; j++)
			a[i][j] = ans[i][j];
	n--, m--;
}
signed main() {
	cin >> n >> m;
	for (int i = 1; i <= n; i++)
		for (int j = 1; j <= m; j++)
			cin >> a[i][j];
	cin >> op;
	for (char l = 'A'; l < op; l++) calc();
	cout << n << " " << m << endl;
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= m; j++) cout << a[i][j] << " ";
		puts("");
	}
	return 0;
}
