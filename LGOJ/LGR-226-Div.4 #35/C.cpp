#include<bits/stdc++.h>
using namespace std;
string s;
int main() {
	cin >> s;
	if (s.size() & 1) s = "0" + s;
	for (int i = 0; i < s.size(); i += 2) {
		swap(s[i], s[i + 1]);
	}
	while (s.size() > 1 && s.back() == '0') s.pop_back();
	reverse(s.begin(), s.end());
	cout << s;
}
