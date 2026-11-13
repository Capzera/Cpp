#include<bits/stdc++.h>
using namespace std;
struct Tree {
	int fail;
	int mp[26];
	int end;
} tr[1000000];
int cnt, n;
string s;
inline void Build(string s) {
	int node = 0;
	for (auto& ch : s) {
		int sn = ch - 'a';
		if (tr[node].mp[sn] == 0) tr[node].mp[sn] = ++cnt;
		node = tr[node].mp[sn];
	}
	tr[node].end++;
}
void Get_fail() {
	queue<int> q;
	for (int i = 0; i < 26; ++i) { 
		if (tr[0].mp[i]) {
			tr[tr[0].mp[i]].fail = 0;
			q.push(tr[0].mp[i]);
		}
	}
	while (q.size()) {
		int u = q.front(); q.pop();
		for (int i = 0; i < 26; ++i) {
			if (tr[u].mp[i] != 0) {
				tr[tr[u].mp[i]].fail = tr[tr[u].fail].mp[i];
				q.push(tr[u].mp[i]);
			} else tr[u].mp[i] = tr[tr[u].fail].mp[i];
		}
	}
}
int AhoCorasickAutomaton(string s) {
	int node = 0, ans = 0;
	for (auto& ch : s) {
		node = tr[node].mp[ch - 'a'];
		for (int t = node; t && tr[t].end != -1; t = tr[t].fail) {
			ans += tr[t].end;
			tr[t].end = -1;
		}
	}
	return ans;
}
int main() {
	cin >> n;
	for (int i = 1; i <= n; ++i) {
		cin >> s;
		Build(s);
	}
	tr[0].fail = 0;
	Get_fail();
	cin >> s;
	cout << AhoCorasickAutomaton(s);
	return 0;
}
