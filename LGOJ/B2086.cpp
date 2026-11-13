#include <bits/stdc++.h>
using namespace std;
long long a, b, c, ans;
int main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	cin >> a >> b >> c;
	for (int x = 1; x <= 1000; x++){
		if (a * x > c) break;
		int sum = c - a * x;
		if (sum % b != 0) continue;
		ans++;
	} 
	cout << ans + (c % b == 0);
	return 0;
}
