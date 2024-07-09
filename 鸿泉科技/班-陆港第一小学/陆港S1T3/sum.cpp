#include <bits/stdc++.h>
using namespace std;
int main() {
	int n, m;
	cin >> n >> m;
	long long s1 = 0, s2 = 0;
	for (int i = 1; i <= n; i++) s1 += i;
	for (int i = 1; i <= m; i++) s2 += i;
	cout << s1 - s2;
	return 0;
}
