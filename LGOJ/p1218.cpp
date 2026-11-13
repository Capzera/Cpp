#include <bits/stdc++.h>
using namespace std;
long long ans;
int n;
bool isPrime(long long x) {
	if (x == 1) return 0;
	if (x == 2 || x == 3) return 1;
	if (x % 6 != 1 && x % 6 != 5) return 0;
	long long n = sqrt(x);
	for (int i = 5; i <= n; i += 6) {
		if (x % i == 0 || (x % (i + 2)) == 0) return 0;
	}
	return 1;
}
void dfs(int idx) {
	if (idx == n) {
		cout << ans << endl;
		return;
	}
	if (idx == 0) {
		ans = 2;
		dfs(1);
		ans = 0;
	}
	for (int i = 0; i < 5; i++) {
		long long nxt = ans * 10 + 2 * i + 1;
		if (isPrime(nxt) == 0) continue;
		ans = nxt;
		dfs(idx + 1);
		ans /= 10;
	}
	
}
int main() {
	cin >> n;
	dfs(0);
	return 0;
}
