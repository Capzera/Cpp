#include <bits/stdc++.h>
using namespace std;
long long n, l, r, k = 1;
int a[100010], b[100010], ans[100010];
int main() {
	//cin.tie(nullptr)->sync_with_stdio(0);
	cin >> n >> l >> r;
	for (int i = 1; i <= n; i++) cin >> a[i];
	for (int i = 1; i <= n; i++) cin >> b[i];
	sort(a + 1, a + n + 1), sort(b + 1, b + n + 1);
	priority_queue<int> pq;
	if (n <= 50 && l != 1) {
		for (int i = 1; i <= n; i++) {
			for (int j = 1; j <= n; j++) {
				ans[k++] = a[i] + b[j];
			}
		}
		sort(ans + 1, ans + n * n + 1);
		for (int i = l; i <= r; i++) cout << ans[i] << " ";
		return 0;
	}
	if (l == 1) {
		for (int i = 1; i <= n; i++) {
			for (int j = 1; j <= n; j++) {
				int x = a[i] + b[j];
				if (pq.size() < r) {
					pq.push(x);
					continue;
				}
				if (x > pq.top()) break;
				pq.push(x);pq.pop();
			}
		}
		while(pq.size()) {
			ans[k++] = pq.top(); pq.pop();
		}
		for (int i = r; i > 0; i--) cout << ans[i] << " ";
		return 0;
	}
	if (l == r) {
		int x = sqrt(l);
		
	}
	return 0;
}
/*
1 + 3 + 5 + 7
3 1 9
2 6 6
1 4 8
3 6 7 7 10 10 10 14 14
7
*/
