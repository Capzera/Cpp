#include <bits/stdc++.h>
#define ll long long
using namespace std;
int main(){
    cin.tie(nullptr)->sync_with_stdio(false);
    vector<vector<ll>> grid(100, vector<ll> (100));
    grid[0][0] = 1;
    for (int a0 = 1; a0 <= 300; a0++) {
        for (int m = 1; m <= 100; m++) {
            for (int n = 1; n <= 100; n++) {
                int b1 = (a0 + n - 1) * n;
                int bn = (b1 + b1 + (m - 1) * n) * m / 2;
                if (bn == 2022) {
                    cout << a0 << " " << m << " " << n << endl;
                    return 0;
                }
            }
        }
    }
    return 0;
}
