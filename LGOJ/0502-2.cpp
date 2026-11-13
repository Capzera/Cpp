#include <bits/stdc++.h>
using namespace std;
const int N = 1e6 + 10;
int n, pi[N];
long long ans;
string s;
int main() {
	cin >> n >> s;
	s = "#" + s;
	for (int i = 2, j = 0; i <= n; i++) {
		while (j && s[i] != s[j + 1]) j = pi[j];
		if (s[i] == s[j + 1]) j++;
		pi[i] = j;
	}
	for (int i = 1; i <= n; i++) {
		if (!pi[i]) continue;
		int j = pi[i];
		while (pi[j]) j = pi[j];
		pi[i] = j;
		ans += i - pi[i];
	}	
	cout << ans;
	return 0;
}
//babababa     i  pi
//b         0  1  0
//ba        0  2  0
//bab       2  3  1
//baba      2  4  2
//babab     4  5  1
//bababa    4  6  2
//bababab   6  7  1
//babababa  6  8  2
//s
//border的border还是border
