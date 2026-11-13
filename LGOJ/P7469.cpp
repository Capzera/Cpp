#include <bits/stdc++.h>
#define ull unsigned long long
using namespace std;
int n;
string a, b;
unordered_set<ull> mp;
int main() {
	cin >> n >> a >> b;
	for (int st = 0; st < n; st++) {
		ull hs = 0;
		for (int i = st, j = 0; i < n; i++, j++) {
			while (j < n && b[i] != a[j]) j++;
			if (j == n) break;
			hs = hs * 131 + b[i];
			mp.insert(hs);
		}
	}
	cout << mp.size();
	return 0;
}
