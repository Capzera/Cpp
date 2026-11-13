#include <bits/stdc++.h>
using namespace std;
int mp1[26], mp2[26];
int main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	string a, b;
	cin >> a >> b;
	for (auto& x : a) mp1[x - 'a']++;
	for (auto& x : b) mp2[x - 'a']++;
	long long cnt = 0;
	for (int i = 0; i < 26; i++) {
		cnt += min(mp1[i], mp2[i]);
	}
	cout << cnt;
	return 0;
}
