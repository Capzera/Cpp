#include <bits/stdc++.h>
using namespace std;
int main() {
    int n, m;
    cin >> n >> m;
    for (int k = 1; ; k++) {
        long long now = k * n + m, flag = 1;
        for (int j = 1; j < n; j++) {
            if (now % ())
        }
    }
    cout << ans << endl;
    system("pause");
    return 0;
}
/*
1 1 2 2 1 1
1 1 2 2 1 1
2 2 4 4 2 2
2 2 4 4 2 2
1 1 2 2 1 1
1 1 2 2 1 1

(m - 4)(n - 4) * 4 + (n - 4) * 4 * 2 + (m - 4) * 4 * 2 + 8
1 1 2 2 2 2 2 2 1 1
1 1 2 2 2 2 2 2 1 1
2 2 0 0 0 0 0 0 2 2
2 2 0 0 0 0 0 0 2 2
2 2 0 0 0 0 0 0 2 2
2 2 0 0 0 0 0 0 2 2
2 2 0 0 0 0 0 0 2 2
2 2 0 0 0 0 0 0 2 2
1 1 0 0 0 0 0 0 1 1
1 1 2 2 2 2 2 2 1 1

*/