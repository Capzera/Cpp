#include <bits/stdc++.h>
using namespace std;
long long n, ans, sum, cnt, lt, rt;
string s;
int main() {
	cin >> n >> s;
	for (auto& x : s) {
		if (x == 'L') {
			cnt++;
		} else {
			sum += cnt;
			rt++;
		}
	}
	ans = sum;
	for (auto& x : s) {
		if (x == '?') {
			sum -= (lt++) - (--rt);
			ans = max(ans, sum);
		}
		if (x == 'L') lt++;
		if (x == 'Q') rt--;
	}
	cout << ans;
	return 0;
}
