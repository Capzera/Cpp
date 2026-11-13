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
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= n + 1; j++) {
			a[i][j] = read();
		}
	}
	for (int i = 1; i <= n; i++) {
		int mx = i;
		for (int j = i + 1; j <= n; j++) 
			if (fabs(a[j][i]) > fabs(a[mx][i])) mx = j;
		swap(a[i], a[mx]);
		if (fabs(a[i][i]) < 1e-6) return puts("No Solution"), 0;
		for (int j = i + 1; j <= n; j++) {
			double d = a[j][i] / a[i][i];
			for (int k = i; k <= n + 1; k++) {
				a[j][k] -= d * a[i][k];
			}
		}
	}
	for (int i = n; i > 0; i--) {
		for (int j = i + 1; j <= n; j++) {
			a[i][n + 1] -= a[i][j] * a[j][n + 1];
		}
		a[i][n + 1] /= a[i][i];

	}
	for (int i = 1; i <= n; i++) printf("%.2lf\n", a[i][n + 1]);
	return 0;
}

