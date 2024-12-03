#include <bits/stdc++.h>
using namespace std;
int n, ans;
int pd(int x) {
	
	for (int i = 2; i * i <= x; i++) {
		if (x % i == 0) return 0;
	}
	return 1;
}
int main() {
	cin >> n;
	for (int i = 2; i <= n; i++) {
		ans += pd(i);
	}
	cout << ans;
}
