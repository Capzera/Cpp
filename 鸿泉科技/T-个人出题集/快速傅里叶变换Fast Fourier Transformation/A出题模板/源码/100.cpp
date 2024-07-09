#include <bits/stdc++.h>
using namespace std;
const double PI2 = 2 * acos(-1);
const int N = 2100000;
int t, n, m, tr[N];
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
int main() {
	cin >> t;
	for (int i = 0; i <= n; i++) cin >> f[i].x;
	for (int i = 0; i <= m; i++) cin >> g[i].x;
	for (m += n, n = 1; n <= m; n <<= 1);
	for (int i = 0; i < n; i++)
		tr[i] = (tr[i >> 1] >> 1) | ((i & 1) ? n >> 1 : 0);
	fft(f, 1);
	fft(g, 1);
	for (int i = 0; i < n; i++) f[i] = f[i] * g[i];
	fft(f, 0);
	for (int i = 0; i <= m; i++) {
		cout << (int)(f[i].x / n + 0.49) << " ";
	}
	return 0;
}
