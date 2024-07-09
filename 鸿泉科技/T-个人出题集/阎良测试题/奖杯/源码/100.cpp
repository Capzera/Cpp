#include <bits/stdc++.h>
using namespace std;
int main() {
    string s;
    cin >> s;
    int ans = -1;
    for (int i = 0; i < s.size() - 1; i++) {
        if (s[i] == 'R' && s[i + 1] == 'L') {
        	ans = 0;
			break;	
		}
        if (s[i] == 'L' && s[i + 1] == 'R') {
        	ans = i + 1;
        	break;
		}
    }
    cout << ans << endl;
    return 0;
}
