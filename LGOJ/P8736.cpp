#include <bits/stdc++.h>
using namespace std;
string s;
int main() {
	cin >> s;
	vector<string> name, dp, ans;
	for (auto& ch : s) {
		string t;
		t += ch;
		if (isupper(ch)) name.push_back(t);
		else name.back() += ch;
	}
	int n = name.size(), len = 0;
	dp = ans = vector<string>(n + 1);
	for (auto& word : name) {
		int j = lower_bound(dp.begin(), dp.begin() + len + 1, word) - dp.begin();
		len = max(len, j);
		dp[j] = word;
		ans[j] = ans[j - 1] + word;
	}
	cout << ans[len - 1];
	return 0;
}
