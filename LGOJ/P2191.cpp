#include <bits/stdc++.h>
using namespace std;
char x, s[1001][1001];
int n, a[1001][1001];
void rotate() {
	int tmp[n + 1][n + 1];
	for (int i = 1; i <= n; i++) 
		for (int j = 1; j <= n; j++)
			tmp[i][j] = a[n - j + 1][i];
	for (int i = 1; i <= n; i++)
		for (int j = 1; j <= n; j++)
			a[i][j] = tmp[i][j];
}
int main() {
	cin >> n;
	for (int i = 1; i <= n; i++) 
		for (int j = 1; j <= n; j++) {
			cin >> x;
			a[i][j] = x == 'O';
		}
	for (int i = 1; i <= n; i++) 
		for (int j = 1; j <= n; j++) 
			cin >> s[i][j];
	for (int d = 0; d < 4; d++) {
		for (int i = 1; i <= n; i++)
			for (int j = 1; j <= n; j++)
			if (a[i][j]) cout << s[i][j];
		rotate();
	}
	return 0;
}
