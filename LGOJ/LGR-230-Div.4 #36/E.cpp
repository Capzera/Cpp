#include <bits/stdc++.h>
using namespace std;
int n, x;
int get(int x) {
	int ans = 0;
	while (x) {
		ans += x % 10;
		x /= 10;
	}
	return ans;
}
int main() {
	cin >> n;
	for (int i = 1; i <= n; i++) {
		cin >> x;
		int y = i;
		for (int j = 1; j <= min(x, 3); j++) {
			y = get(y);
		}
		cout << y << " ";
	}
	return 0;
}
