#include <bits/stdc++.h>
using namespace std;
const int N = 2e6 + 5;
int n, L[N], R[N], mpR[N], mpL[N];
long long d, x[N], dpL[N], dpR[N];
long long solve() {
	cin >> n >> d;
	for (int i = 1; i <= n; i++) {
		cin >> x[i];
		mpR[i] = mpL[i] = 0;
		dpL[i] = dpR[i] = 0;
	}
	for (int i = 1, j = 1; i <= n; i++) {
		while (j < n && x[j + 1] - x[i] <= d) j++;
		R[i] = j;
	}
	for (int i = n, j = n; i >= 1; i--) {
		while (j > 1 && x[i] - x[j - 1] <= d) j--;
		L[i] = j;
	}
	for (int i = 1; i <= n; i++) mpR[R[i]]++, mpL[L[i]]++;
	for (int i = 1; i <= n; i++) {
		dpL[i] = mpR[i];
		if (L[i] >= 1 && R[L[i]] == i) dpL[i] += dpL[L[i]];
	}
	for (int i = n; i >= 1; i--) {
		dpR[i] = mpL[i];
		if (R[i] <= n && L[R[i]] == i) dpR[i] += dpR[R[i]];
	}
	long long ans = 0;
	for (int i = 1; i < n; i++) {
		long long lt = mpR[i + 1]; 
		if (L[i] >= 1 && R[L[i]] == i + 1) lt += dpL[L[i]];
		lt -= (R[i] == i + 1) + (R[i + 1] == i + 1);
		long long rt = mpL[i];
		if (R[i + 1] <= n && L[R[i + 1]] == i) rt += dpR[R[i + 1]];
		rt -= (L[i] == i) + (L[i + 1] == i);
		if (lt > 0 && rt > 0) ans += lt * rt;
	}
	return ans;
}
int main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	int c, t;
	cin >> c >> t;
	while (t--) cout << solve() << '\n';
	return 0;
}
