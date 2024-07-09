/*
交替字符串 字符串-数学-思维  同小低组T3
*/
#include <bits/stdc++.h>
using namespace std;
int main() {
	string s;
	cin >> s;
	int n = s.size();
	long long ans = 0;
	for (int i = 0, j = 0; i < n; i++) {
		if (i && s[i] == s[i - 1]) j = i;
		ans += i - j + 1;
	}
	cout << ans << endl; 
	return 0;
}
