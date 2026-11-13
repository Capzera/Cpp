#include <bits/stdc++.h>
using namespace std;
unordered_set<int> s;
int n, a[100010];
int main() {
	cin >> n;
	a[1] = 1;
	s.insert(1);
	for (int i = 2; i <= n; i++) {
		int k = a[i - 1] - i;
		if (k > 0 && !s.count(k)) a[i] = k;
		else a[i] = a[i - 1] + i;
		s.insert(a[i]);
	}
	sort(a + 1, a + n + 1);
	for (int i = 1; i <= n; i++) {
		cout << a[i] << " ";
	}
	return 0;
}
