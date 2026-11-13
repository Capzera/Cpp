#include <bits/stdc++.h>
using namespace std;
string s;
int n, p, mask;
int main() {
	cin >> s >> p;
	n = s.size();
	vector<int> now;
	for (int i = 0, f = 0, j = -1; i < n; i++, j = -1) {
		if (s[i] == '(') f = 1;
		if (s[i] == ')') {
			now.push_back(mask);
			mask = 0;
			f = 0;
		} 
		if (isalpha(s[i])) j++;
		if (s[i] == 'R') j += 2;
		if (i + 1 < n && s[i + 1] == '\'') i++, j++;
		if (!f && j >= 0) now.push_back(1 << j);
		if (f && j >= 0) mask |= 1 << j;
	}
	cout << now.size() << endl;
	if (p) {
		for (int x = 0; x < 4; x++) {
			int ans = 0, cnt = 0;
			for (auto& mask : now) {
				if (mask >> x & 1) ans = max(ans, ++cnt);
				else cnt = 0;
			}
			cout << max(ans, cnt) << " ";
		}
		return 0;
	}
}
