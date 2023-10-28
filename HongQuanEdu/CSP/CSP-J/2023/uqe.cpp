/*

*/
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
ll g, w, p, q, s;
void print(ll x, ll y) {
    g = __gcd(x, y); x /= g, y /= g;
    if (!x) return;
    if (y < 0) x = -x, y = -y;
    if (y == 1) cout << x;
    else cout << x << "/" << y;
}
int main() {
    int T, M;
    cin >> T >> M;
    while (T--) {
        int a, b, c;
        cin >> a >> b >> c;
        int delta = 1LL * b * b - 4 * a * c;
        w = sqrt(delta);
        if (delta < 0) {
            cout << "NO" << endl;
            continue;
        }
        if (w * w == delta) {
            p = -b + (a < 0 ? -w : w);
            print(p, 2 * a);
            if (!p) cout << "0";
            cout << endl;
            continue;
        }
        print(-b, 2 * a);
        if (b) cout << "+";
        p = 1, q = 2 * a, s = delta;
        for (int i = w; i >= 2; i--) {
            if (s % (i * i) == 0) {
                p *= i;
                s /= (i * i);
            }
        }
        g = __gcd(p, q);
        p = abs(p / g), q = abs(q / g);
        if (p == 1 && q == 1) cout <<"sqrt(" << s << ")" << endl;
        else if (p == 1) cout << "sqrt(" << s << ")/" << q << endl;
        else if (q == 1) cout << p << "*sqrt(" << s << ")" << endl;
        else cout << p << "*sqrt(" << s << ")/" << q << endl;
    }
    system("pause");
    return 0;
}