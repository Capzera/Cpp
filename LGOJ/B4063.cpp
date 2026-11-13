#include <bits/stdc++.h>
using namespace std;
int n, ans[2], x;
int main() {
	cin >> n;
	while (n--) {
		cin >> x;
		ans[x & 1]++;
	}
	cout << ans[1] << " " << ans[0];
	return 0;
}
