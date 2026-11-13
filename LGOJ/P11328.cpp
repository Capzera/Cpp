#include <bits/stdc++.h>
using namespace std;
long long n, cur, ans;
priority_queue<int> pq;
struct node {
	long long l, x;
	bool operator < (const struct node& rhs) {
		return l + x < rhs.l + rhs.x;
	}
};
int main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	cin >> n;
	vector<node> a(n);
	for (auto& v : a) cin >> v.x;
	for (auto& v : a) cin >> v.l;
	sort(a.begin(), a.end());
	for (int i = 0; i < n; i++) {
		if (cur <= a[i].l) {
			cur += a[i].x;
			pq.push(a[i].x);
		} else if (pq.size() && a[i].x < pq.top()) {
			cur += a[i].x - pq.top(); pq.pop();
			pq.push(a[i].x);
		}
	}
	cout << pq.size();
	return 0;
}
