#include <bits/stdc++.h>
using namespace std;
int x, n, ans;
struct cho {
	mutable int a, b, c;
} v[100010];
bool cmp1 (cho x, cho y) {
	return x.a > y.a;
}
bool cmp2 (cho x, cho y) {
	return x.b < y.b;
}
int main() {
	cin >> x >> n;
	priority_queue<cho, vector<cho>, decltype(&cmp1)> pq(cmp1);
	for (int i = 1; i <= n; i++) {
		cin >> v[i].a >> v[i].b >> v[i].c;
	}
	sort(v + 1, v + n + 1, cmp2);
	for (int i = x, j = n; i; i--) {
		while (i <= v[j].b) {
			pq.push(v[j--]);
		}
		ans += pq.top().a;
		if (!(--pq.top().c)) {
			pq.pop();
		}
	}
	cout << ans;
	return 0;
}
