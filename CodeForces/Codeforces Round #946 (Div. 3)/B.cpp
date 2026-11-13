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
	string s;
	set<char> se;
	int n;
	cin >> n >> s;
	for (auto & ch : s) {
		se.insert(ch);
	}
	string r(se.begin(), se.end());
	reverse(r.begin(), r.end());
	unordered_map<char, char> um;
	for (int i = 0, j = r.size() - 1; i <= j; i++, j--) {
		um[r[i]] = r[j];
		um[r[j]] = r[i];
	}
	for (auto& ch : s) {
		ch = um[ch];
	}
	print(s);
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
