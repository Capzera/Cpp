#include <bits/stdc++.h>
#define pic pair<int, char>
using namespace std;
int n, x, t;
void solve() {
	cin >> n;
	priority_queue<pic> pq;
	int sum = 0;
	for (int i = 1; i <= n; i++) {
		cin >> x;
		pq.push({x, 'A' + i - 1});
		sum += x;
	}
	auto op = [&]()->void {
		pic x = pq.top(); pq.pop();
		cout << x.second;
		x.first--, sum--;
		if (x.first > 0) pq.push(x);
	};
	while (pq.size()) {
		op();
		if (pq.size()) {
			pic x = pq.top(); 
			if (pq.size() && pq.top().first * 2 > sum) op();
		}
		cout << " ";
	}
}

int main() {
	cin >> t;
	for (int i = 1; i <= t; i++) {
		printf("Case #%d: ", i);
		solve();
		puts("");
	}
	return 0;
}
