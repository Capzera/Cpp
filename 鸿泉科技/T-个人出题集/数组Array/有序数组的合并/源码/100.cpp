#include <bits/stdc++.h>
using namespace std;
int a[101], b[101], c[205];
int main() {
	int n, m;
	cin >> n >> m;

	int i = 1, j = 1, k = 1;
	for (int i = 1 ; i <= n; i++) cin >> a[i];
	for (int j = 1; j <= m; j++) cin >> b[j];
	while (i <= n && j <= m) {
		if (a[i] <= b[j]) {
			c[k++] = a[i++];
		} else c[k++] = b[j++];
	}
	while (i <= n) c[k++] = a[i++];
	while (j <= m) c[k++] = b[j++];
	for (int i = 1; i <= n + m; i++) cout << c[i] << " ";
	return 0;
}
