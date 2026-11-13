#include <bits/stdc++.h>
using namespace std;
const int N = 5e5 + 10;
int n, k, top, stk[N];
string s;
int main() {
	cin >> n >> k >> s;
	for (auto& x : s) {
		while (k && top && stk[top] < x - '0') {
			k--, top--;
		}
		stk[++top] = x - '0';
	}
	for (int i = 1; i <= top - k; i++) printf("%d", stk[i]);
	return 0;
}
