#include <bits/stdc++.h>
using namespace std;
int nums[100001], award[100001], n, m;

bool check(int x) {
    int l = 0, r = m;
    while (l < r) {
        int mid = l + (r - l) / 2;
        if (award[mid] == x) return 1;
        if (award[mid] < x) l = mid + 1;
        else r = mid;
    }
    return 0;
}

int main() {
    cin >> n >> m;
    for (int i = 0; i < n; i++) cin >> nums[i];
    for (int i = 0; i < m; i++) cin >> award[i];
    sort(award, award + m);
    for (int i = 0; i < n; i++) {
        if (check(nums[i])) cout << nums[i] << " ";
    }
    system("pause");
    return 0;
}