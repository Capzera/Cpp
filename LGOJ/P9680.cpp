#include <bits/stdc++.h>
using namespace std;
int main() {
    int T, ans = 0;
	cin >> T;
    map<string, int> mp; 
	while(T--){
        string s, t, valName;
		cin >> s >> t; 
        int i = 0, n = t.size();
		for (; i < n; i++) {
            if (t[i] == '(') break;
            valName += t[i];
        }
        t = t.substr(i + 1, n - i - 3);
        int len = t.size();
        if (t[0] == '\"') len -= 2;
        else len = mp[t];
		mp[valName] = len;
		if (s == "string") ans += len;
	} 
	cout << ans << endl;
	return 0;
}
