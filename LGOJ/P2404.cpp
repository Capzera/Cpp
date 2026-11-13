#include <bits/stdc++.h>
using namespace std;
int n, a[9];
void dfs(int idx, int sum, int lst) {
	if (sum >= n) {
		if (sum == n) {
			for (int i = 0; i < idx; i++) {
				cout << a[i];
				cout << (i < idx - 1 ? '+' : '\n');
			}
		}
		return;
	}
	for (int i = lst; i < n; i++) {
		if (sum + i > n) break;
		a[idx] = i;
		dfs(idx + 1, sum + i, i);
	}
}
int main() {
	cin >> n;
	dfs(0, 0, 1);
	return 0;
}
