#include<bits/stdc++.h>
using namespace std;
int t;
set<set<string>> ans;
string s;
int main() {
	cin >> t;
	while (t--) {
		cin >> s;
		set<string> mp;
		string t;
		for (auto& ch : s) {
			if (islower(ch)) t += ch;
			else mp.insert(t), t = "";
		}
		ans.insert(mp);
	}
	cout << ans.size();
	return 0;
}
