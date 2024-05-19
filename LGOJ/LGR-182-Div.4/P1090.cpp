#include <bits/stdc++.h>
using namespace std;
int main() {
	int n, x;
	cin >> n;
	long long ans = 0;
	priority_queue<int, vector<int>, greater<int> > pq;
	for (int i = 0; i < n; i++) {
		cin >> x;
		pq.push(x);
	}
	for (int i = 1; i < n; i++) {
		int merge = 0;
		merge += pq.top(); pq.pop();
		merge += pq.top(); pq.pop();
		pq.push(merge);
		ans += merge;
	}
	cout << ans << endl;
	return 0;
}
