#include <bits/stdc++.h>
using namespace std;
int n;
string s;
void dfs(int i) {
	if (i >= n) {
		cout << s << '\n';
		return;
	}
	for (int j = 0; j < 2; j++) {
		s += !j ? 'N' : 'Y';
		dfs(i + 1);
		s.pop_back();
	}
}
int main() {
	cin >> n;
	dfs(0);
	return 0;
}
NNN 000
NNY 001
NYN 010
NYY 011
YNN 100
YNY 101
YYN 110
YYY 111


