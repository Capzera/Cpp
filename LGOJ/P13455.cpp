#include <bits/stdc++.h>
using namespace std;
int T, t, na, nb, h1, m1, h2, m2, a[101], b[101];
char ch;
void solve(int test) {
	priority_queue<int, vector<int>, greater<int> > pq1, pq2; // 两个最小堆维护最早完成周转的列车时间
	int ca = 0, cb = 0; // 统计答案
	cin >> t >> na >> nb;
	for (int i = 0; i < na; i++) {
		cin >> h1 >> ch >> m1 >> h2 >> ch >> m2;
		a[i] = h1 * 60 + m1; // A 站出发时间， h * 60 + m 将小时制转为分钟制
		pq2.push(h2 * 60 + m2 + t); // 维护到达 B 站并周转完成的最早列车
	}
	for (int i = 0; i < nb; i++) {
		cin >> h1 >> ch >> m1 >> h2 >> ch >> m2;
		b[i] = h1 * 60 + m1; // B 站出发时间
		pq1.push(h2 * 60 + m2 + t); // 维护到达 A 站并周转完成的最早列车
	}
	sort(a, a + na), sort(b, b + nb); // 对出发时间从小到大排序
	for (int i = 0; i < na; i++) {
		if (pq1.size() && pq1.top() <= a[i]) { // 若有车可用(完成调度的时间 <= 出发时间)
			pq1.pop(); // 使用该车
		} else {
			ca++; // 增加列车
		}
	}
	for (int i = 0; i < nb; i++) {
		if (pq2.size() && pq2.top() <= b[i]) {
			pq2.pop();
		} else {
			cb++;
		}
	}
	printf("Case #%d: %d %d\n", test, ca, cb);
}
int main() {
	cin >> T;
	for (int i = 1; i <= T; i++) solve(i);
	return 0;
}
