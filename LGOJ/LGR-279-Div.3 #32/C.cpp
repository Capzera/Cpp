#include <bits/stdc++.h>
using namespace std;
inline int read() {
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
long long t, n, k;
string s;
long long solve() {
	n = read();
	cin >> s;
	long long sum = 0, slide = 0;
	for (auto& x : s) sum += x - '0';
	priority_queue<int> pq;
	for (int i = 0; i < n; i++) {
		int num = s[i] - '0';
		pq.push(num * 9);
		slide += num * 9;
		if (pq.size() > (i + 1 >> 1)) {
			int mx = pq.top(); pq.pop();
			slide -= mx;
		}
	}
	return 10 * sum - slide;
}
int main() {
	t = read();
	while (t--) cout << solve() << '\n';
	return 0;
}

