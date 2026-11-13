#include <bits/stdc++.h>
using namespace std;
const int N = 1e5 + 10;
int n, ans, a[N];
int main() {
	cin >> n;
	for (int i = 0; i < n; i++) cin >> a[i];
	sort(a, a + n, greater<int>());
	for (int i = 0; i < n; i++) {
		if (a[i] < i) return cout << i, 0;
	}
	cout << n;
	return 0;
}
