#include <bits/stdc++.h>
using namespace std;
const double PI2 = 2 * acos(-1);
const int N = 2100000;
int n, m, tr[N], carry;
string a, b;
struct comp {
	double x, y;
	comp (double _x = 0, double _y = 0) : x(_x), y(_y) {}
	comp operator + (comp const &B) const {
		return comp(x + B.x, y + B.y);
	}
	comp operator - (comp const &B) const {
		return comp(x - B.x, y - B.y);
	}
	comp operator * (comp const &B) const {
		return comp(x * B.x - y * B.y, x * B.y + y * B.x);
	}
} f[N], g[N];
void fft(comp *F, bool flag) {
	for (int i = 0; i < n; i++)
		if (i < tr[i])
			swap(F[i], F[tr[i]]);
	for (int p = 2; p <= n; p <<= 1) {
		int k = p >> 1;
		comp ur(cos(PI2 / p), sin(PI2 / p));
		if (!flag) ur.y *= -1;
		for (int i = 0; i < n; i += p) {
			comp fl(1, 0);
			for (int j = i; j < i + k; j++) {
				comp fr = fl * F[j + k];
				F[j + k] = F[j] - fr;
				F[j] = F[j] + fr;
				fl = fl * ur;
			}
		}
	}
}
string mulFFT(string &a, string& b) {
	n = a.size() - 1, m = b.size() - 1;
	for (int i = 0; i <= n; i++) f[i].x = a[n - i] - '0';
	for (int j = 0; j <= m; j++) g[j].x = b[m - j] - '0';
	for (m += n, n = 1; n <= m; n <<= 1);
	for (int i = 0; i < n; i++)
		tr[i] = (tr[i >> 1] >> 1) | ((i & 1) ? n >> 1 : 0);
	fft(f, 1);
	fft(g, 1);
	for (int i = 0; i < n; i++) f[i] = f[i] * g[i];
	fft(f, 0);
	string ans;
	for (int i = 0; i <= m; i++) {
		carry += (int)(f[i].x / n + 0.5);
		ans += carry % 10 + '0';
		carry /= 10;
	}
	while (carry) {
		ans += carry % 10 + '0';
		carry /= 10;
	}
	while (ans.size() > 1 && ans.back() == '0') ans.pop_back();
	reverse(ans.begin(), ans.end());
	return ans;
}
int main() {
	cin >> a >> b;
	cout << mulFFT(a, b);
	return 0;
}
