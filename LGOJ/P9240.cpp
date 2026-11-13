#include <bits/stdc++.h>
using namespace std;
int n, x, y, mn, mx = INT_MAX;
int main() {
	cin >> n;
	while (n--) {
		cin >> x >> y;
		mx = min(mx, x / y);
		mn = max(mn, (x + y) / (y + 1));
	}
	cout << mn << " " << mx;
	return 0; 
}
