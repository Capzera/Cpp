#include <bits/stdc++.h>
using namespace std;
const int dir[] = {-1, 1, -4, 4};
long long ori, tgt, mp[65536];
char x;
queue<int> q;
int main() {
	for (int i = 0; i < 16; i++) {
		cin >> x;
		if (x == '1') ori |= 1 << i;
	}
	for (int i = 0; i < 16; i++) {
		cin >> x;
		if (x == '1') tgt |= 1 << i;
	}
	memset(mp, -1, sizeof(mp));
	mp[ori] = 0;
	q.push(ori);
	while (q.size()) {
		int cur = q.front(); q.pop();
		if (cur == tgt) {
			cout << mp[tgt];
			return 0;
		}
		for (int i = 0; i < 16; i++) {
			if (cur >> i & 1 ^ 1) continue;
			for (int d = 0; d < 4; d++) {
				if (i % 4 == 0 && d == 0) continue;
				if (i / 4 == 0 && d == 2) continue;
				if (i % 4 == 3 && d == 1) continue;
				if (i / 4 == 3 && d == 3) continue;
				int j = i + dir[d];
				if ((cur >> i & 1) == (cur >> j & 1)) continue;
				int nxt = cur ^ (1 << i) ^ (1 << j);
				if (mp[nxt] != -1) continue;
				mp[nxt] = mp[cur] + 1;
				q.push(nxt);
			}
		}
	}
	return 0;
}
