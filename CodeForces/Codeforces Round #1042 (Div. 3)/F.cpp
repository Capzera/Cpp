#include <bits/stdc++.h>
#define ll long long
#define pii pair<int, ll>
using namespace std;
const int N = 2e5 + 10;
const ll inf = 1 << 60;
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
	return f * ans;
}
struct treearray {
	vector<ll> sum, g;
	int n;
	treearray(int _n) {
		n = _n;
		sum = g = vector<ll>(n + 1);
	}
	void update(int x, int c, ll val) {
		for (; x <= n; x += x & -x) {
			sum[x] += c;
			g[x] += val;
		}
	}
	pair<ll, ll> query(int x) {
		if (x <= 0) return {0, 0};
		if (x > n) x = n;
		ll cnt = 0, val = 0;
		for (; x; x -= x & -x) {
			cnt += sum[x];
			val += g[x];
		}
		return {cnt, val};
	}
};
int n, A[N], B[N];
long long f[N], g[N];
string a, b;
void solve() {
	cin >> n >> a >> b;
	A[1] = 0;
	for (int i = 2; i <= n; i++) {
		A[i] = A[i - 1] + ((a[0] - 48) ^ (a[i - 1] - 48));
	}
	B[1] = (a[0] - '0') ^ (b[0] - '0');
	for (int j = 2; j <= n; j++) {
		B[j] = B[j - 1] + ((a[0] - '0') ^ (b[j - 1] - '0'));
	}
	ll s1 = 0, s2 = 0, ans = 0;;
	for (int j = 1; j <= n; j++) {
		s1 += B[j];
	}
	memset(f, 0, sizeof(f));
	memset(g, 0, sizeof(g));
	vector<int> fs;
	for (int i = 1; i <= n; i++) {
		f[i] = 2 * B[i] - i;
		g[i] = i - 2 * B[i];
		s2 += g[i];
		fs.push_back(f[i]);
	}
	sort(fs.begin(), fs.end());
	fs.erase(unique(fs.begin(), fs.end()), fs.end());
	int m = fs.size();
	treearray fen(m);
	for (int y = 1; y <= n; y++) {
		int val = f[y];
		int pos = lower_bound(fs.begin(), fs.end(), val) - fs.begin();
		fen.update(pos + 1, 1, g[y]);
	}
	for (int x = 1; x <= n; x++) {
		int T = x - 2 * A[x];
		int pos0 = lower_bound(fs.begin(), fs.end(), T + 1) - fs.begin();
		auto [cnt0, sumG0] = fen.query(pos0);
		ll cnt = n - cnt0;
		ll sumG_part = s2 - sumG0;

		ll total_x = (ll)n * A[x] + s1;
		total_x += (ll)(x - 2 * A[x]) * cnt + sumG_part;
		ans += total_x;
	}
	cout << ans << '\n';
}

int main() {
	int t = 1;
	t = read();
	while (t--) solve();
	// while(t--) cout << (solve() ? "Yes\n" : "No\n");
	return 0;
}
