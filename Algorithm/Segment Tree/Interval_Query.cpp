#include <bits/stdc++.h>
using namespace std;
class SegmentTree {
private:
    vector<int> Tree;//l,r的区间和
    vector<int> Lazy;//懒惰标记，对区间修改情况
    int n;
public:
    SegmentTree(){};
    SegmentTree(vector<int>& nums) {
        this -> n = nums.size();
        Lazy = Tree = vector<int> (n * 4);
        build(0, 0, n - 1, nums);
    }

    void update(int l, int r, int inc) {
        change(0, 0, n - 1, inc, l, r);
    }

    int query(int l, int r) {
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

    void build(int node, int l, int r, vector<int>& nums) {
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

    int sumRange(int node, int l, int r, int left, int right) {
        if (left <= l && right >= r) return Tree[node];
        push_down(node, l, r);
        int mid = l + (r - l) / 2, ans = 0;
        if (left <= mid) ans += sumRange(node * 2 + 1, l, mid, left, right);
        if (right > mid) ans += sumRange(node * 2 + 2, mid + 1, r, left, right);
        return ans;
    }
    ~SegmentTree(){};
};

int main() {
    int n, q;
    cin >> n >> q;
    vector<int> nums(n);
    for (int i = 0; i < n ; i++) cin >> nums[i];
    SegmentTree T(nums);
    while (q--) {
        int x, y, o;
        cin >> x >> y >> o;
        if (o)
            T.update(x, y, o);
        else cout<< T.query(x, y) << endl;
    }
    system("pause");
    return 0;
}