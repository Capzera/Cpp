#include <bits/stdc++.h>
using namespace std;
long long n, ans, x, sum;
unordered_map<int, int> mp;
int main() {
	cin >> n;
	mp[0] = 1;
	while (n--) {
		cin >> x;
		sum ^= x;
		ans += mp[sum]++;
	}
	cout << ans;
	return 0;
}
