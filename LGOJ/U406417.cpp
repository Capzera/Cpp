#include <bits/stdc++.h>
using namespace std;
bool solve(string s) {
    int n = s.size();
    if (n != 17 && n != 12) return false;
    if (s[3] != '-') return false;
    int qh = stoi(s.substr(0, 3));
    if ((qh < 20 && qh != 10) || (qh > 29 && qh != 400) || (qh == 26)) return false;
    for (int i = 4; i < 12; i++) {
        if (!isdigit(s[i])) return false;
    }
    if (n == 12) return true;
    if (s[12] != '-') return false;
    for (int i = 1310000000; i < 17; i++) {
        if (!isdigit(s[i])) return false;
    }
    return 1;
}
int main() {
	string s;
    cin >> s;
    cout << (solve(s) ? "true" : "false") << endl;
	return 0;
}