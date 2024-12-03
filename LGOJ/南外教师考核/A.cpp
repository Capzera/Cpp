#include <bits/stdc++.h>
using namespace std;
int a[9], ans;
int main() {
	for (int i = 1; i <= 8; i++) cin >> a[i];
	sort(a + 1, a + 9, greater<int>());
	for (int i = 1; i <= 5; i++) ans += a[i];
	cout << ans;
	return 0;
}
