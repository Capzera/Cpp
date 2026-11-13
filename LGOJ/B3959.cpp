#include <bits/stdc++.h>
using namespace std;
int n, ans;
int main() {
	cin >> n;
	vector<int> a(n);
	for (auto& x : a) cin >> x;
	sort(a.begin(), a.end());
	for (auto& x : a) ans += x > ans;
	cout << ans;
	return 0;
}
/*

100 5
3 10  ans = 30, cnt = 10
5 20  ans = 130, cnt = 30
6 30  ans = 310, cnt = 60
8 80  ans = 630, cnt = 100
9 40
*/
