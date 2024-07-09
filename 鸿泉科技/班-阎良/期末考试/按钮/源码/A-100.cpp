#include <bits/stdc++.h>
using namespace std;
string s;
long long ans;
int main() {
    cin >> s;
    for (int i = 0; i < s.size(); i++) {
    	if (s[i] >= 'a' && s[i] <= 'z') {
    		s[i] -= 32;
		}
	}
    for (int i = 1; i < s.size(); i++) {
        if (s[i] != s[i - 1]) {
        	ans++;
		}
    }
    cout << ans << endl;
    return 0;
}
