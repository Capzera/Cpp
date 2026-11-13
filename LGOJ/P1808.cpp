#include <bits/stdc++.h>
using namespace std;
int n;
string s;
unordered_set<string> mp;
int main() {
	cin >> n;
	while (n--) {
		cin >> s;
		sort(s.begin(), s.end());
		mp.insert(s);
	}
	cout << mp.size();
	return 0;
}
