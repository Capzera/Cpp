#include <bits/stdc++.h>
#define ll long long
#define pii pair<int, int>
using namespace std;
const int N = 1e5 + 10;
int read() {
	int ans = 0, f = 1;
	char ch = getchar();
	while (!isdigit(ch)) {
		if (ch == '-') f = -1;
		ch = getchar();
	}
	while (isdigit(ch)) {
		ans = ans * 10 + ch - '0';
		ch = getchar();
	}
	return ans * f;
}

void solve() {
	
}

int main() {
	int t = 1;
	cin >> t;
	for (int i = 1; i <= t; i++) {
		printf("Case #%d: ", i);
		solve();
		printf("\n");
	}
	return 0;
}
