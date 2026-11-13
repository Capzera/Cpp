#include <bits/stdc++.h>
using namespace std;
string s;
int n, vis[101];
stack<int> stk;
int main() {
	cin >> s;
	n = s.size();
	for (int i = 0; i < n; i++) {
		if (s[i] == '(' || s[i] == '[') {
			stk.push(i);
			continue;
		}
		if (stk.size()) {
			int t = stk.top();
			if ((s[i] == ')' && s[t] == '(') || (s[i] == ']' && s[t] == '[')) {
				vis[t] = vis[i] = 1;
				stk.pop();
			}
		}
	}
	for (int i = 0; i < n; i++) {
		if (vis[i]) {
			cout << s[i];
		} else if (s[i] < 42) cout << "()";
		else cout << "[]";
	}
	return 0;
}

