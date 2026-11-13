#include <bits/stdc++.h>
using namespace std;
string a, b;
int z[40000001];
void exKMP(string s) {
	int l = 0, n = s.size();
	for (int i = 1; i < n; i++) {
		if (l + z[l] > i) z[i] = min(z[i - l], l + z[l] - i);
		while (i + z[i] < n && s[z[i]] == s[i + z[i]]) z[i]++;
		if (i + z[i] > l + z[l]) l = i;
	}
}
long long ansz, ansp;
int main() {
	cin >> a >> b;
	int la = a.size(), lb = b.size();
	exKMP(b + a);
	ansz = lb + 1;
	for (int i = 1; i < lb; i++) {
		ansz ^= 1ll * (min(z[i], lb - i) + 1) * (i + 1);
	}
	for (int i = 0; i < la; i++) {
		ansp ^= 1ll * (min(z[i + lb], lb) + 1) * (i + 1);
	}
	cout << ansz << '\n' << ansp;
	return 0;
}
