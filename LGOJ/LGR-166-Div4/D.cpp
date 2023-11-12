#include <bits/stdc++.h>
using namespace std;
int main() {
    int n;
    cin >> n;
    auto check = [&](int x) -> int {
        if (x == 2 || x == 3 || x == 5) return 1;
        if (x % 6 != 1 && x % 6 != 5) return 0;
        for (int i = 5; i * i <= x; i += 6) {
            if (x % i == 0 || x % (i + 2) == 0) return 0;
        }
        return 1;
    };
    for (int i = 1; i <= n; i++) {
        int cnt = 0, col = 1;
        int alpha = i - 1;
        while (cnt < i * i) {
            int t = alpha + 65 + col;
            if (!check(t)) cout << ' ';
            else {
                cout << char('A' + alpha);
                alpha = (alpha + 1) % 26;
                cnt++;
            }
            col++;
        }
        cout << endl;
    }
    system("pause");
    return 0;
}