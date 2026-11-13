#include <bits/stdc++.h>
using namespace std;
long long n, m, x, sum;
priority_queue<int, vector<int>, greater<int> > pq;
int main() {
	cin >> n >> m;
	for (int i = 1; i <= n; i++) {
		cin >> x;
		sum += x;
		pq.push(x);
		while (pq.size() && sum - pq.top() >= m) {
			sum -= pq.top(); pq.pop();
		}
		cout << (sum < m ? -1 : int(pq.size())) << " ";
	}
	return 0;
}
