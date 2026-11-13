#include <bits/stdc++.h>
using namespace std;
long long n, m, sum, ans, a[200001];
int main() {
	cin >> n >> m;
	for (int i = 1; i <= n; i++) cin >> a[i];
	sort(a + 1, a + n + 1);
	if (a[1] != 1) return puts("No answer!!!"), 0;
	while (sum < m) {
		for (int i = n; i > 0; i--) {
			if (a[i] > sum + 1) continue;
			ans++;
			sum += a[i];
			break;
		}
	}
	cout << ans;
	return 0;
}
