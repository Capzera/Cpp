#include <bits/stdc++.h>
using namespace std;
int n, x;
priority_queue<int> gpq;
priority_queue<int, vector<int>, greater<int> > lpq;
int main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	cin >> n >> x;
	gpq.push(x);
	cout << x << '\n';
	for (int i = 2; i <= n; i++) {
		cin >> x;
		if (x > gpq.top()) lpq.push(x);
		else gpq.push(x);
		while (abs((int)gpq.size() - (int)lpq.size()) > 1) {
			if (gpq.size() > lpq.size()) {
				lpq.push(gpq.top()); gpq.pop();
			} else {
				gpq.push(lpq.top()); lpq.pop();
			}
		}
		if (i % 2 != 0) cout << (gpq.size() > lpq.size() ? gpq.top() : lpq.top()) << '\n';
	}
	return 0;
}


 1 - n   连通表示两人认识
n + 1- 2n 连通表示两人不认识


总有6个人
1-6   7-12

3, 8

第3个人和第2个人不认识


3 和9是连通的 无解
u, u + v   v, v+ n


merge(u, v)
merge(u + n, v) merge(u, n + v);



u, u + n,    v, v + n

