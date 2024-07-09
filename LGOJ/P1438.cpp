#include <bits/stdc++.h>
using namespace std;
class SegmentTree {
private:
	vector<long long> Tree;//l,r的区间和
	vector<long long> Lazy;//懒惰标记，对区间修改情况
	int n;
public:
	SegmentTree(){};
	SegmentTree(vector<long long>& nums) {
		this -> n = nums.size();
		Lazy = Tree = vector<long long> (n * 4);
		build(0, 0, n - 1, nums);
	}
	
	void update(int l, int r, int inc) {
		change(0, 0, n - 1, inc, l, r);
	}
	
	long long query(int l, int r) {
		return sumRange(0, 0, n - 1, l, r);
	}
	
	void push_down(int node, int l, int r) {
		if (!Lazy[node]) return;
		int mid = l + (r - l) / 2;
		Lazy[node * 2 + 1] += Lazy[node];
		Lazy[node * 2 + 2] += Lazy[node];
		Tree[node * 2 + 1] += Lazy[node] * (mid - l + 1);
		Tree[node * 2 + 2] += Lazy[node] * (r - mid);
		Lazy[node] = 0;
	}
	
	void push_up(int node) {
		Tree[node] = Tree[node * 2 + 1] + Tree[node * 2 + 2];
	}
	
	void build(int node, int l, int r, vector<long long>& nums) {
		if (l == r) {
			Tree[node] = nums[l];
			return;
		}
		int mid = l + (r - l) / 2;
		build(node * 2 + 1, l, mid, nums);
		build(node * 2 + 2, mid + 1, r, nums);
		push_up(node);
	}
	
	void change(int node, int l, int r, int inc, int left, int right) {//left right待操作，l,r当前结点
		if (left <= l && right >= r) {
			Tree[node] += (r - l + 1) * inc;
			Lazy[node] += inc;//node的孩子待加inc的值，但是现在暂时不管
			return;
		}
		push_down(node, l, r);
		int mid = l + (r - l) / 2;
		if (left <= mid) change(node * 2 + 1, l, mid, inc, left, right);
		if (right > mid) change(node * 2 + 2, mid + 1, r, inc, left, right);
		push_up(node);
	}
	
	long long sumRange(int node, int l, int r, int left, int right) {
		if (left <= l && right >= r) return Tree[node];
		push_down(node, l, r);
		long long mid = l + (r - l) / 2, ans = 0;
		if (left <= mid) ans += sumRange(node * 2 + 1, l, mid, left, right);
		if (right > mid) ans += sumRange(node * 2 + 2, mid + 1, r, left, right);
		return ans;
	}
	~SegmentTree(){};
};

long long n, m, op, l, r, k, d, x;
int main() {
	cin >> n >> m;
	vector<long long> a(n + 1), diff(n + 1);
	for (int i = 1; i <= n; i++) {
		cin >> a[i];
		diff[i] = a[i] - a[i - 1];
	}
	diff.push_back(0);
	SegmentTree T(diff);
	while (m--) {
		cin >> op;
		if (op == 1) {
			cin >> l >> r >> k >> d;
			T.update(l, l, k);
			if (l + 1 <= r) T.update(l + 1, r, d);
			if (r < n) T.update(r + 1, r + 1, -(k + d * (r - l)));
		} else {
			cin >> x;
			cout << T.query(1, x) << endl;
		}
	}
	return 0;
}
