#include <bits/stdc++.h>
using namespace std;
struct node {
	int st, lst, id;
	bool operator < (const struct node& rhs) const {
		return id > rhs.id;
	}
} x, a[100001];
bool cmp(const node& x, const node& y) {
	return x.st < y.st;
}
int n, ans;
priority_queue<node> pq;
int main() {
	cin >> n;
	for (int i = 1; i <= n; i++) {
		cin >> a[i].st >> a[i].lst;
		a[i].id = i;
	}
	sort(a + 1, a + n + 1, cmp);
	int ed = a[1].st + a[1].lst;
	for (int i = 2; i <= n; i++) {
		if (a[i].st < ed) { // 当前奶牛的开始吃草的时间小于结束时间
			pq.push(a[i]); // 当前奶牛加入等待队列
			continue;
		}
		if (pq.empty()) { // 没有奶牛在等待，直接开吃
			ed = a[i].st + a[i].lst;
			continue;
		}
		ans = max(ans, ed - pq.top().st);
		ed += pq.top().lst; pq.pop();
		if (a[i].st >= ed) i--; // 仍然有奶牛在等待
		else pq.push(a[i]);
	}
	while (pq.size()) {
		ans = max(ans, ed - pq.top().st);
		ed += pq.top().lst; pq.pop();
	}
	cout << ans;
	return 0;
}
