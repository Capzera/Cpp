#include <bits/stdc++.h>
using namespace std;
long long n, cur;
priority_queue<long long> pq; 
struct node {
	long long need, cost;
	bool operator < (const struct node& rhs) {
		return need + cost <= rhs.need + rhs.cost;
	}
} a[300010];
int main() {
	cin >> n;
	for (int i = 1; i <= n; i++) cin >> a[i].need >> a[i].cost;
	sort(a + 1, a + n + 1);
	for (int i = 1; i <= n; i++) {
		if (cur <= a[i].need) {
			cur += a[i].cost;
			pq.push(a[i].cost);
		} else if (pq.size() && pq.top() > a[i].cost) {
			cur += a[i].cost - pq.top(), pq.pop();
			pq.push(a[i].cost);
		}
	}
	cout << pq.size() << endl;
	return 0;
}
