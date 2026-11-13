#include <bits/stdc++.h>
using namespace std;
string s;
int n, len, ans = INT_MAX;
unordered_set<char> mp; // 统计字符串中共多少种字符
unordered_map<char, int> slide; // 窗口内的字符情况
int main() {
	cin >> n >> s;
	for (auto& ch : s) mp.insert(ch);
	len = mp.size();
	for (int i = 0, j = 0; i < n; i++) {
		slide[s[i]]++; // 右端入窗口
		while (j < i && slide.size() == len) {
			ans = min(ans, i - j + 1);
			slide[s[j]]--;
			if (!slide[s[j]]) slide.erase(s[j]);
			j++;
		}
	}
	cout << ans;
	return 0;
}
