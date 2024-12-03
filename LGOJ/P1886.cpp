#include <bits/stdc++.h>
using namespace std;
deque<int> mn, mx;
int n, k, x, a[1000010];
vector<int> ans;
int main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	cin >> n >> k;
	for (int i = 1; i <= n; i++) {
		cin >> a[i];
		while (mx.size() && a[i] >= a[mx.back()]) mx.pop_back();
		while (mn.size() && a[i] <= a[mn.back()]) mn.pop_back();
		mx.push_back(i), mn.push_back(i);
		while (i - k >= mx.front()) mx.pop_front();
		while (i - k >= mn.front()) mn.pop_front();
		if (i >= k) {
			cout << a[mn.front()] << " ";
			ans.push_back(a[mx.front()]);
		}
	}
	cout << endl;
	for (auto& x : ans) cout << x << " ";
	return 0;
}
