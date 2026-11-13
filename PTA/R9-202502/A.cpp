#include <bits/stdc++.h>
using namespace std;
int n;
string s;
map<string, int> mp;
int main() {
	cin >> n;
	while (n--) {
		cin >> s;
		mp[s]++;
	}
	cout << 40 - mp.size();
	return 0;
}
