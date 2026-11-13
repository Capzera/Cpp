#include <bits/stdc++.h>
using namespace std;
long long x;
int main() {
	vector<long long> num[2];
	for (int i = 0; i < 10; i++) {
		cin >> x;
		num[x & 1].push_back(x);
	}
	sort(num[1].begin(), num[1].end(), greater<long long>());
	reverse(num[0].begin(), num[0].end());
	for (auto& x : num[1]) cout << x << " ";
	for (auto& x : num[0]) cout << x << " ";
	return 0;
}
