#include <bits/stdc++.h>
using namespace std;
const int N = 5e5 + 10;
int n, a[N];
long long M(int l, int r) {
	if (abs(r - l) > 5) {
		return M(l, l + r >> 1) % max(M((l + r >> 1) + 1, r), 7ll) + (a[l + r >> 1] - 1);
	}
	return *max_element(a + l, a + r + 1);
}
int main() {
	cin >> n;
	for (int i = 1; i <= n; i++) cin >> a[i];
	cout << M(1, n);
	return 0;
}
