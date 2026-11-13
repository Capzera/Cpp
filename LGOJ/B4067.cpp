#include <bits/stdc++.h>
using namespace std;
string s;
int main() {
	vector<vector<string> > mp = {
		{
			".....",
			".***.",
			".***.",
			".***.",
			"....."
		}, {
			"****.",
			"****.",
			"****.",
			"****.",
			"****.",
		}, {
			".....",
			"****.",
			".....",
			".****",
			".....",
		}, {
			".....",
			"****.",
			".....",
			"****.",
			".....",
		}
	};
	cin >> s;
	for (int i = 0; i < 5; i++) {
		string ans;
		for (int j = 0; j < s.size(); j++) {
			ans += mp[s[j] - '0'][i];
		}
		cout << ans << endl;
	}
	return 0;
}
