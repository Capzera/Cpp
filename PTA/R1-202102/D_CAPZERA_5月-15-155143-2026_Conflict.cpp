#include <bits/stdc++.h>
using namespace std;
const int N = 2e5 + 10, K = 60;
inline int read() {
	long long ans = 0;
	char ch = getchar();
	while (!isdigit(ch)) ch = getchar();
	while (isdigit(ch)) {
		ans = ans * 10 + ch - '0';
		ch = getchar();
	}
	return ans;
}
int n, k, p, a[N], b[N], cnt[K], mp[K];
long long ans;
int main() {
	n = read(), k = read(), p = read();
	for (int i = 1; i <= n; i++) {
		a[i] = read(), b[i] = read();
		cnt[a[i]]++;
		if (!mp[a[i]]) mp[a[i]] = i;
	}
	for (int j = 0; j < k; j++) {
		if (cnt[j] == 1) continue;
		int mn = a[mp[j]], sum = 0;
		for (int i = mp[j] + 1; i <= n; i++) {
			mn = min(mn, b[i]);
			if (a[i] != j) continue;
			sum++;
			if (mn > p) continue;
			ans += cnt[j] - sum;
			break;
		}
	}
	cout << ans;
	return 0;
}
