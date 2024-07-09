#include <bits/stdc++.h>
using namespace std;
long long n, x, y, a[200005], b[200005], sum[200005];
long long answer1, answer2;
bool cmp(int a, int b) {
	return a > b;
}
void a1() {
	sort(a + 1, a + n + 1, cmp);
	for (int i = 1; i <= y; i++) {
		a[i] *= -1;
	}
	for (int i = 1; i <= n; i++) {
		answer1 += a[i];
	}
}
void a2() {
	sort(b + 1, b + n + 1, cmp);
	for (int i = 1; i <= x; i++) {
		b[i] = 0;
	}
	for (int i = x + 1; i <= x + 1 + y; i++) {
		b[i] *= -1;
	}
	for (int i = 1; i <= n; i++) {
		answer2 += b[i];
	}
}
int main() {
	cin >> n >> x >> y;
	for (int i = 1; i <= n; i++) {
		cin >> a[i];
		b[i] = a[i];
		sum[i] = sum[i - 1] + a[i];
	}
	a1();
	a2();
	cout << max(answer1, answer2);
	return 0;
}
