#include <bits/stdc++.h>
using namespace std;
long long n, a[5010];
int days[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
int cal(int y, int m, int d, int f, int s) {
	int ans = d + 365 * (y - 1);
	for (int yy = 0; yy < y; yy++) {
		ans += yy % 400 == 0 || yy % 4 == 0 && yy % 100 != 0;
	}
	for (int mm = 1; mm < m; mm++) {
		ans += days[mm];
	}
	return ans * 1440 + f * 60 + s;
}
int main() {
	int y1, m1, d1, f1, s1, y2, m2, d2, f2, s2, day1, day2, diff;
	cin >> n;
	for (int i = 0; i < n; i++) cin >> a[i];
	sort(a, a + n);
	scanf("%d-%d-%d-%d:%d", &y1, &m1, &d1, &f1, &s1);
	scanf("%d-%d-%d-%d:%d", &y2, &m2, &d2, &f2, &s2);
	day1 = cal(y1, m1, d1, f1, s1);
	day2 = cal(y2, m2, d2, f2, s2);
	diff = day2 - day1;
	for (int i = 0; i < n; i++) {	
		if (diff < a[i]) {
			cout << i << endl;
			return 0;
		}
		diff -= a[i];
	}
	cout << n;
	return 0;
}
