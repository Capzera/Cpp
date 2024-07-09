#include <bits/stdc++.h>
using namespace std;
int main() {
	int n, s, ans = 0;
	cin >> n;
	for (int i = 0; i < n; i++) {
		cin >> s;
		ans = max(ans, s);
	}
	cout << ans << endl;
	return 0;
}
