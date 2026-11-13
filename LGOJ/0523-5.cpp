#include <bits/stdc++.h>
using namespace std;
int n, m, a[100][100];

bool cmp(int lhs[100], int rhs[100]) {
	for (int i = 1; i <= m; i++) {
		if (lhs[i] == rhs[i]) continue;
		return lhs[i] < rhs[i];
	}
	return 1;
}

int main() {
	cin >> n >> m;
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= m; j++) {
			cin >> a[i][j];
		}
	}
	sort(a + 1, a + n + 1, cmp);
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= m; j++) {
			cout << a[i][j] << " ";
		}
		cout << endl;
	}
	return 0;
}

