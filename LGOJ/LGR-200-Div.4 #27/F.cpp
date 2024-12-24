#include <bits/stdc++.h>
using namespace std;
string s;
long long ans;
int main() {
	cin >> s;
	for (int i = 0; i < s.size(); i++)
		for (int j = i; j < s.size(); j++)
			ans += s[i] == s[j];
	cout << ans;
	return 0;
}
