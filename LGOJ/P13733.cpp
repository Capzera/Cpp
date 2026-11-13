#include <bits/stdc++.h>
using namespace std;
const int N = 3e5 + 10;
int n, k, x, cnt = 1;
set<int> s;
int main() {
	cin >> n >> k;
	for (int i = 1; i <= n; i++) {
		cin >> x;
		s.insert(x);
	}
	vector<int> a(s.begin(), s.end());
	for (int i = 1; i < a.size(); i++) {
		if (a[i] - a[i - 1] == 1) cnt++;
		else cnt = 1;
		if (cnt == k) return puts("Yes"), 0;
	}
	cout << "No";
	return 0;
}
