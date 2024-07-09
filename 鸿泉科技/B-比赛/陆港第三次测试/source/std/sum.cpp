#include <bits/stdc++.h>
using namespace std;
int main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	long long n, m;
	cin >> n >> m;
	long long s1 = n * (n + 1) / 2, s2 = m * (m + 1) / 2;
	cout << s1 - s2;
	return 0;
}
