#include <bits/stdc++.h>
using namespace std;
const int N = 105;
int n, x;
int solve() {
	cin >> n;
	int sum[2] = {0};
	while (n--) cin >> x, sum[x & 1]++;
	if (sum[1] % 4 == 3 || sum[1] % 4 == 0) return 1;
	if (sum[1] % 4 == 2) return 0;
	return sum[0] & 1;
}

int main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	int T = 1;
	cin >> T;
	while (T--) cout << (solve() ? "Alice\n" : "Bob\n");
	return 0;
}
