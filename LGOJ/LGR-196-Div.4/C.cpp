#include <bits/stdc++.h>
using namespace std;
long long n, x, s1, s2;
int main() {
	cin >> n;
	for (int i = 0; i < n; i++) cin >> x, s1 += x;
	for (int i = 0; i < n; i++) cin >> x, s2 += x;
	long long ans = (s1 + s2 + 1) / 2;
	cout << ans;
	return 0;
}
