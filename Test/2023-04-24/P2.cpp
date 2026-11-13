#include<bits/stdc++.h>
using namespace std;
void merge(int a[], int b[], int n, int m) {
    int len = m + n, k = len - n;
    for (int i = 0, j = k; i < n; i++) a[j++] = a[i];
    int l = k, r = m - 1, i = 0;
    while (l < len && r >= 0) {
        if (a[l] < ¡¢b[r]) {
            a[i++] = a[l++];
        }
        else a[i++] = a[r--];
    }
    while (l < len) a[i++] = a[l++];
    while (r >= 0) a[i++] = a[r--];
    for (int i = 0; i < len; i++) printf("%d ", a[i]);
}
int main() {
    int m, n;
    cin >> m >> n;
    int a[m + n], b[n];
    for (int i = 0; i < m; i++) cin >> a[i];
    for (int j = 0; j < n; j++) cin >> b[j];
    merge(a, b, m, n);
    system("pause");
    return 0;
}