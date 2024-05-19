#include <bits/stdc++.h>
using namespace std;
int main() {
	int n, t = 1;
	long long ans = 0;
	cin >> n;
	vector<vector<int> > cows(n, vector<int>(2));
	priority_queue<int, vector<int>, greater<int> > pq;
	for (auto& cow : cows) cin >> cow[0] >> cow[1];
	sort(cows.begin(), cows.end());
	for (auto& cow : cows) {
		if (cow[0] < t) {
			int x = pq.top();
			if (x < cow[1]) {
				pq.pop();
				pq.push(cow[1]);
				ans += cow[1] - x;
			}
		} else {
			pq.push(cow[1]);
			ans += cow[1];
			t++;
		}
	}
	cout << ans << endl;
	return 0;
}
