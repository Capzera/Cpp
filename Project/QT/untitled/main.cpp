#include <bits/stdc++.h>
#if defined(_WIN32) || defined(_WIN64)
#define getchar_unlocked _getchar_nolock
#endif
using namespace std;

inline int read() {
    int ans = 0;
    char ch = getchar_unlocked();
    while (!isdigit(ch)) ch = getchar_unlocked();
    while (isdigit(ch)) {
        ans = ans * 10 + ch - '0';
        ch = getchar_unlocked();
    }
    return ans;
}

class SegmentTree {
private:
    vector<bool> tr, lz;
    int n;
    void push_down(int node, int l, int r) {
        if (lz[node]) {
            tr[node << 1] = tr[node << 1 | 1] = 1;
            lz[node << 1] = lz[node << 1 | 1] = 1;
            lz[node] = 0;
        }
    }

    void push_up(int node) {
        tr[node] = tr[node << 1] & tr[node << 1 | 1];
    }

    void updateAss(int node, int l, int r, int lt, int rt) {
        if (l >= lt && r <= rt) {
            tr[node] = lz[node] = 1;
            return ;
        }
        push_down(node, l, r);
        int mid = (l + r) >> 1;
        if (lt <= mid) updateAss(node << 1, l, mid, lt, rt);
        if (rt > mid) updateAss(node << 1 | 1, mid + 1, r, lt, rt);
        push_up(node);
    }

    int queryAnd(int node, int l, int r, int lt, int rt) {
        if (l >= lt && r <= rt) return tr[node];
        push_down(node, l, r);
        int mid = l + ((r - l) >> 1);
        bool ans = 1;
        if (lt <= mid) ans = ans & queryAnd(node << 1, l, mid, lt, rt);
        if (rt > mid) ans = ans & queryAnd(node << 1| 1, mid + 1, r, lt, rt);
        return ans;
    }

public:
    SegmentTree(int n) {
        this->n = n;
        tr = lz = vector<bool>(4 * n + 5);
    }

    void update(int l, int r) {
        updateAss(1, 1, n, l, r);
    }

    int query(int l, int r) {
        return queryAnd(1, 1, n, l, r);
    }
};

int n, m, ans;

int main() {
    n = read(), m = read();
    vector<int> a(m + 1), b(m + 1);
    for (int i = 1; i <= m; i++) {
        a[i] = read(), b[i] = read();
        if (a[i] > b[i]) swap(a[i], b[i]);
    }
    SegmentTree st(n);

    for (int i = m; i > 0; i--) {
        if (!st.query(a[i], b[i])) ans++;
        st.update(a[i], b[i]);
    }
    cout << ans;
    return 0;
}
