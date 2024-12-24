#include <bits/stdc++.h>
using namespace std;
int n, w, d, ans = 97;
int main() {
	cin >> n;
	while(n--) {
		cin >> w >> d;
		ans = min(ans, d * 7 - 6 + w);
	}
	cout << ans;
	return 0;
}
