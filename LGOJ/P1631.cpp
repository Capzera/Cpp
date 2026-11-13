#include <bits/stdc++.h>
using namespace std;
int main() {
	int n;
	cin >> n;
	vector<int> a(n), b(n);
	priority_queue<int, vector<int>, greater<int>> pq;
	for (auto& x : a) cin >> x;
	for (auto& x : b) cin >> x;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n && i * j < n; j++) {
			pq.push(a[i] + b[j]);
		}
	}
	while (n--) {
		cout << pq.top() << " ";
		pq.pop();
	}
	return 0;
}
