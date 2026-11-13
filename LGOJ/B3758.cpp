#include <bits/stdc++.h>
using namespace std;
int top, stk[1001];
string s, ans;
int main() {
	cin >> s;
	for (auto& ch : s) {
		if (ch == '(') ans += "(", stk[++top] = 1;
		else {
			if (!top) ans += "()";
			else ans += ")", top--;
		}
	}
	while (top--) ans += ")";
	cout << ans;
	return 0;
}

string 
1 5
3 7
