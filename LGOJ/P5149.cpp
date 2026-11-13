#include <bits/stdc++.h>
using namespace std;
long long n, ans, a[100001], tmp[100001];
struct Trie {
	int val;
	unordered_map<char, Trie*> mp;
	Trie() : val(0) {}
} *root;
void build(string s, int v) {
	auto p = root;
	for (auto& ch : s) {
		p = p->mp[ch] ? p->mp[ch] : p->mp[ch] = new Trie();
	}
	p->val = v;
}
int query(string s) {
	auto p = root;
	for (auto& ch : s) {
		p = p->mp[ch];
	}
	return p->val;
}
string s;
void mergesort(int l, int r) {
	if (l >= r) return;
	int mid = l + (r - l >> 1);
	mergesort(l, mid), mergesort(mid + 1, r);
	int i = l, j = mid + 1, k = 0;
	while (i <= mid && j <= r) {
		if (a[i] > a[j]) {
			ans += mid - i + 1;
			tmp[k++] = a[j++];
		} else tmp[k++] = a[i++];
	}
	while (i <= mid) tmp[k++] = a[i++];
	while (j <= r) tmp[k++] = a[j++];
	for (int i = l; i <= r; i++) a[i] = tmp[i - l];
}
int main() {
	cin >> n;
	root = new Trie();
	for (int i = 1; i <= n; i++) {
		cin >> s;
		build(s, i);
	}
	for (int i = 1; i <= n; i++) {
		cin >> s;
		a[i] = query(s);
	}
	mergesort(1, n);
	cout << ans;
	return 0;
}
