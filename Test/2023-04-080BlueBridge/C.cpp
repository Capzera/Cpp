#include <bits/stdc++.h>
using namespace std;
int main() {
    int n, a, b;
    cin >> n;
    int minv = 0, maxv = INT_MAX;
    for (int i = 0; i < n; i++) {
        cin >> a >> b;
        minv = max(minv, a / (b + 1));
        maxv = min(maxv, a / b);
    }
    cout << minv + 1  << " " << maxv << endl;
    system("pause");
    return 0;
}