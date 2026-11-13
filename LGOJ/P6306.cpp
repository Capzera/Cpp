#include<bits/stdc++.h>
using namespace std;
inline long long read() {
	long long x = 0, ch = getchar();
	for (; ch < '0' || ch > '9'; ch = getchar());
	for (; ch >= '0' && ch <= '9'; ch = getchar()) x = (x << 1) + (x << 3) + (ch ^ 48);
	return x;
}
int a[6][256];
long long ans, n, k, x;
int main() {
	n = read(), k = read();
	while (n--) {
		x = read();
		for (int i = 0; i < 6; i++) a[i][(x >> (i << 3)) & 255]++;
	}
	for (long long i = 1; i < 256; i++)
		for (int j = 0; j < 6; j++)
			if (a[j][i] % k) ans += i << (j << 3);
	cout << ans;
	return 0;
}
