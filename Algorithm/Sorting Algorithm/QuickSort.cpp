#include<bits/stdc++.h>
using namespace std;
vector<int> a;
void qsort(int l, int r) {
    if (l >= r) return; // 如果当前递归区间仅有1个元素，则退出
    int i = l, j = r, x = a[(l + r) / 2]; // i,j左右指针，x是基准值
    while (i <= j) { // 保证左指针在右指针的左边
        while (a[i] < x) i++; // 在基准值左侧找到大于基准值的值
        while (a[j] > x) j--; // 在基准值右侧找到小于基准值的值
        if (i <= j) swap(a[i++], a[j--]); // 如果指针是满足要求的，则交换
    }
    qsort(l, j), qsort(i, r); // 递归，将裂开的两个数组进行排序
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
