#include <bits/stdc++.h>
using namespace std;
unordered_map<char,vector<char> > mp;
string st;
int k, l, mul[100];
vector<int> c(10);
void dfs(char th) {
	c[th - '0'] = 1;
	int n = mp[th].size();
	for(int i = 0; i < n; i++)
		if(!c[mp[th][i]-'0']) dfs(mp[th][i]);
}
int main() {
	cin >> st >> k;
	l = st.length();
	for (int i = 1; i <= k; i++) {
		char x, y;
		cin >> x >> y;
		mp[x].push_back(y);
	}
	mul[0] = 1;
	for (int i = 0; i < l; i++) {
		c = vector(10, 0);
		dfs(st[i]);
		int x = 0, sum = accumulate(c.begin(), c.end(), 0);
		for (int i = 0; i < 100; i++) {
			mul[i] = mul[i] * sum + x;
			x = mul[i] / 10;
			mul[i] %= 10;
		}
	}
	int i = 99;
	while (i > 0 && !mul[i]) i--;
	for (; i >= 0; i--) cout << mul[i];
	return 0;
}
