#include<bits/stdc++.h>
using namespace std;
long long n, ans, cur, lst;
int main() {
	cin >> n;
	for (int i = 1; i <= n; i++) {
		cin >> cur;
		ans += max(cur - lst, 0ll);
		lst = cur;
	}
	cout << ans;
	return 0;
}
