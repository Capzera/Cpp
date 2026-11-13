#include <bits/stdc++.h>
using namespace std;
long long n, cur, ans;
priority_queue<int> pq;
int main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	cin >> n;
	vector<pair<int, int> > a(n);
	for (auto& v : a) cin >> v.second >> v.first;
	sort(a.begin(), a.end());
	for (auto& [need, cost] : a) {
		cur += cost;
		pq.push(cost);
		if (cur > need) cur -= pq.top(), pq.pop();
		else ans++;
	}
	cout << ans;
 	return 0;
}
