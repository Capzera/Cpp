#include<bits/stdc++.h>
using namespace std;
vector<int> a;
void qsort(int l, int r) {
    if (l >= r) return;
    int i = l, j = r, x = a[(l + r) / 2];
    while (i <= j) {
        while (a[i] < x) i++;
        while (a[j] > x) j--;
        if (i <= j) swap(a[i++], a[j--]);
    }
    qsort(i, r), qsort(l, j);
}
int main() {
    int n;
    cin >> n;
    a.resize(n);
    for (auto& x : a) cin >> x;
    qsort(0, n - 1);
    for (auto& x : a) cout<< x << " ";
    system("pause");
    return 0;
}