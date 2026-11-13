#include <bits/stdc++.h>
using namespace std;
struct node {
	int st, ed, id;
	bool operator < (const struct node& rhs) const {
		return st < rhs.st;
	}
} a[1000001];
int n, k, st, ed;
priority_queue<int, vector<int>, greater<int> > pq;
int main() {
	cin >> n >> k;
	for (int i = 1; i <= n; i++) {
		cin >> a[i].st >> a[i].ed;
		a[i].id = i;
	}
	sort(a + 1, a + n + 1);
	for (int i = 1; i <= n; i++) {
		pq.push(a[i].ed);
		if (pq.size() < k) continue;
		if (pq.size() > k) pq.pop();
		if (pq.top() - a[i].st > ed - st) {
			st = a[i].st;
			ed = pq.top();
		}
	}
	cout << ed - st << endl;
	for (int i = 1; i <= n && k; i++) {
		if (st < a[i].st || ed > a[i].ed) continue;
		cout << a[i].id << " ";
		k--;
	}
	return 0;
}
/*
1 10

3 8
4 12
5 9
11 12
*/
