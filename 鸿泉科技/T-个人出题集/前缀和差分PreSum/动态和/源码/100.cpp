#include <bits/stdc++.h>
using namespace std;
int main() {
	int n, sum = 0;
	cin >> n;
	vector<int> arr(n);
	for (auto& x : arr) {
		cin >> x;
		sum += x;
		x = sum;
	}
	for (auto & x : arr) cout << x << " ";
	return 0;
}
