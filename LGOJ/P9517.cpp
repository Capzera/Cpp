#include <bits/stdc++.h>
using namespace std;
int n, x, l = -1, r = -1;
int main() {
	cin >> n;
	for (int i = 1; i <= n; i++) {
		cin >> x;
		if (!x) continue;
		if (l == -1) l = i;
		r = i;
	}
	cout << (r == -1 ? 0 : r - l + 1);
	return 0;
}
