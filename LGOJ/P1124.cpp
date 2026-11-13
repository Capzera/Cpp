#include <bits/stdc++.h>
using namespace std;
string L, ans;
int n, p, nxt[10001], ptr[26];
vector<int> mp[26];
int main() {
	cin >> n >> L >> p; p--;
	string F = L;
	stable_sort(F.begin(), F.end());
	for (int i = 0; i < n; i++) mp[L[i] - 'a'].push_back(i);
	for (int i = 0; i < n; i++) {
		int x = F[i] - 'a';
		nxt[mp[x][ptr[x]++]] = i;
	}
	for (int i = 0; i < n; i++, p = nxt[p]) ans += L[p];
	reverse(ans.begin(), ans.end());
	cout << ans;
	return 0;
}
