#include <bits/stdc++.h>
using namespace std;
int ans, q[14][13], head[14], tail[14];
char num;
int get(char num) {
	if (num == '0') return 10;
	if (isdigit(num)) return num - '0';
	if (num == 'A') return 1;
	if (num == 'J') return 11;
	if (num == 'Q') return 12;
	return 13;
}
int main() {
	for (int i = 1; i <= 13; i++) {
		head[i] = 5, tail[i] = 4;
		for (int j = 1; j <= 4; j++) {
			cin >> num;
			q[i][++tail[i]] = get(num);
		}
	}
	for (int k = 1; k <= 4; k++) {
		int now = q[13][head[13]]; head[13]++;
		while (now != 13) {
			q[now][--head[now]] = now;
			now = q[now][tail[now]--];
		}
	}
	for (int i = 1; i <= 12; i++) {
		ans += head[i] == 1;
	}
	cout << ans;
	return 0;
}
