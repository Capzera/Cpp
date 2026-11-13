#include <bits/stdc++.h>
using namespace std;
vector<string> word[26];
vector<int> cnt[26];
string s;
int n, m;
char ch;
int main() {
	cin >> n >> m;
	while (n--) {
		cin >> s;
		word[s[0] - 'a'].push_back(s);
		cnt[s[0] - 'a'].push_back(0);
	}
	for (auto& w : word) sort(w.begin(), w.end());
	while (m--) {
		cin >> ch;
		int id = ch - 'a', j = 0;
		for (int i = 1; i < word[id].size(); i++) {
			if (cnt[id][i] >= cnt[id][j]) continue;
			j = i;
		}
		cout << word[id][j] << '\n';
		cnt[id][j]++;
	}
	return 0;
}
