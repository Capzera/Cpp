#include <bits/stdc++.h>
using namespace std;
int n;
double a[105][105];
inline int read() {
	int ans = 0, f = 1;
	char ch = getchar();
	while (!isdigit(ch)) {
		if (ch == '-') f = -1;
		ch = getchar();
	}
	while (isdigit(ch)) {
		ans = ans * 10 + ch - '0';
		ch = getchar();
	}
	return ans * f;
}
int main() {
	n = read();
	for (int i = 1; i <= n; i++)
		for (int j = 1; j <= n + 1; j++)
			a[i][j] = read();
	int r = 1;
	for (int c = 1; c <= n; c++) {
		int mx = r;
		for (int j = r + 1; j <= n; j++)
			if (fabs(a[j][c]) > fabs(a[mx][c])) mx = j;
		if (fabs(a[mx][c]) < 1e-6) continue;
		swap(a[r], a[mx]);
		for (int j = 1; j <= n; j++) {
			if (j == r) continue;
			double d = a[j][c] / a[r][c];
			for (int k = c; k <= n + 1; k++)
				a[j][k] -= d * a[r][k];
		}
		r++;
	}
	if (r <= n) {
		for (int i = r; i <= n; i++) {
			if (fabs(a[i][n + 1]) > 1e-6) return puts("-1"), 0;
		}
		return puts("0"), 0;
	}
	for (int i = 1; i <= n; i++) {
		double ans = a[i][n + 1] / a[i][i];
		if (fabs(ans) < 1e-6) ans = 0;
		printf("x%d=%.2lf\n", i, ans);
	}
	return 0;
}
