#include <bits/stdc++.h>
using namespace std;
int main() {
	int n, k;
	cin >> n;
	vector<vector<int> > a(n + 1, vector<int>(3));
	auto cmp = [&](auto& x, auto& y) -> bool {
		return a[x][1] < a[y][1];
	};
	priority_queue<int, vector<int>, decltype(&cmp)> pq;
	for (int i = 1; i <= n; i++) {
		cin >> a[i][0] >> a[i][1];
		a[i][2] = i;
	}
	sort(a.begin() + 1, a.end());
	for (int i = 1; i <= n; i++) {
		pq.push(i);
		if (pq.size() > k) pq.pop();
		if (pq.)
	}
	return 0;
}
/*
1 10
2 6
3 8
4 12
5 9
11 12

*/
