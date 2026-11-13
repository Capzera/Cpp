#include <bits/stdc++.h>
using namespace std;
long long n, x;
set<int> mp;
int main() {
	cin >> n;
	for (int i = 1; i <= n; i++) {
		mp.insert(i);
	}
	while (--n) {
		cin >> x;
		mp.erase(x);
	}
	cout << *mp.begin();
	return 0;
}
