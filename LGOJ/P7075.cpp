#include <bits/stdc++.h>
using namespace std;
const int N = 146097;
long long T, n, Y, M, D;
int yr[N], mn[N], dy[N];
int days[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
void solve() {
	cin >> n;
	if (n <= 2299160) {
		Y = n / 1461 * 4 - 4712;
		D = n % 1461;
	} else {
		n -= 2159351;
		Y = n / N * 400 + 1200;
		D = n % N;
	}
	Y += yr[D];
	cout << dy[D] << " " << mn[D] << " " << max(Y, 1 - Y) << " ";
	if (Y <= 0) cout << "BC";
	puts("");
}
int main() {
	mn[0] = dy[0] = 1;
	for (int i = 1; i < N; i++) {
		dy[i] = dy[i - 1] + 1;
		yr[i] = yr[i - 1], mn[i] = mn[i - 1];
		days[2] = mn[i] == 2 && yr[i] % 400 == 0 || yr[i] % 4 == 0 && yr[i] % 100 != 0 ? 29 : 28;
		if (dy[i] > days[mn[i]]) dy[i] = 1, mn[i]++;
		if (mn[i] > 12) mn[i] = 1, yr[i]++;
	}
	cin >> T;
	while (T--) solve();
	return 0;
}
