#pragma GCC optimize(2)
#include <bits/stdc++.h>
#define inf 0x3f3f3f3f
#define pii pair<int, int>
#define pll pair<long long, long long>
#define umii unordered_map<int, int>
#define umll unordered_map<long long, long long>
#define umsi unordered_map<string, int>
#define ll long long
#define mod 1000000007
#define vi vector<int>
#define vll vector<long long>
#define vvi vector<vector<int>>
#define vvl vector<vector<long long>>
#define vs vector<string>
#define yn(ans) printf("%s\n", (ans)? "Yes" : "No")
#define YN(ans) printf("%s\n", (ans)? "YES" : "NO")
using namespace std;
template <class T>
void read(vector<T> &a) {
	for (int i = 0; i < a.size(); i++) cin >> a[i];
}
template <class T>
void read(T &a) {
	cin >> a;
}
template <class T>
void read(T &a, T &b) {
	cin >> a >> b;
}
template <class T>
void read(vector<vector<T>> &a) {
	int m = a.size();
	if (!m) return;
	int n = a[0].size();
	for (int i = 0; i < m; i++)
		for (int j = 0; j < n; j++) cin >> a[i][j];
}
template <class T>
void print(T n) {
	cout << n << endl;
}
template <class T>
void print(T m, T n) {
	cout << m << " " << n << endl;
}
template <class T>
void print(vector<T> num) {
	for (auto& x : num) cout << x << " ";
	cout << endl;
}
template <class T>
void print(vector<vector<T>> grid) {
	for (auto& i : grid) {
		for (auto& j : i) cout << j << " ";
		cout << endl;
	}
}
int MOD (int x, int y) {
	return x - y * (x / y);
}
ll Abs(ll a) {
	ll b = a >> 31;
	return (a + b) ^ b;
}
template <class T>
T max(T &a, T &b) {
	return a > b ? a : b;
}
template <class T>
T max(T &a, T &b, T &c) {
	return a > b && a > c ? a : (b > a && b > c ? b : c);
}
void solve() {
	int n;
	string s;
	cin >> n >> s;
	ll up = 0, rt = 0;
	ll lup = -1, ldn = -1, llt = -1, lrt = -1;
	for (int i = 0; i < n; i++) {
		if (s[i] == 'N') {
			up++;
			lup = i;
		}
		if (s[i] == 'S') {
			up--;
			ldn = i;
		}
		if (s[i] == 'W') {
			rt--;
			llt = i;
		}
		if (s[i] == 'E') {
			rt++;
			lrt = i;
		}
	}
	if (up % 2 || rt % 2 || (n == 2 && !up && !rt)) {
		cout << "NO" << endl;
		return;
	}
	up /= 2, rt /= 2;
	int upr = 0, rtr = 0, uph = 0, rth = 0;
	vector<char> ans(n, 'Q');
	for (int i = 0; i < n; i++) {
		if (upr < up && s[i] == 'N') {
			upr++;
			ans[i] = 'R';
		}
		if (upr > up && s[i] == 'S') {
			upr--;
			ans[i] = 'R';
		}
		if (rtr < rt && s[i] == 'E') {
			rtr++;
			ans[i] = 'R';
		}
		if (rtr > rt && s[i] == 'W') {
			rtr--;
			ans[i] = 'R';
		}
	}
	ll cnt = 0;
	for (auto& x : ans) {
		if (x == 'Q') {
			x = 'H';
			cnt++;
		}

	}
	if (cnt == n) {
		if (lup != -1 && ldn != -1) {
			ans[lup] = ans[ldn] = 'R';
		} else if (llt != -1 && lrt != -1) {
			ans[llt] = ans[lrt] = 'R';
		}
	}
	for (auto& x : ans) cout << x;
	cout << endl;
}
int main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	int _t = 1;
	cin >> _t;
	while (_t--) {
		solve();
	}
	return 0;
}
