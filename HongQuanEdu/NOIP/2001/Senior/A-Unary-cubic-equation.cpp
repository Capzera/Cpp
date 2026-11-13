/*
    给定一元三次方程，求方程的三个实数根，必定有解

*/
#include <bits/stdc++.h>
using namespace std;
int main() {
    double a, b, c, d;
    double as, bs, t, si;
    double x1, x2, x3;
    cin >> a >> b >> c >> d;
    as = b * b - 3 * a * c;
    bs = b * c - 9 * a * d;
    t = (2 * as * b - 3 * a * bs) / (2 * sqrt(as * as * as));
    si = acos(t);
    x1 = (-b - 2 * sqrt(as) * cos(si / 3)) / (3 * a);
    x2 = (-b + sqrt(as) * (cos(si / 3) + sqrt(3) * sin(si / 3))) / (3 * a);
    x3 = (-b + sqrt(as) * (cos(si / 3) - sqrt(3) * sin(si / 3))) / (3 * a);
    cout << fixed << setprecision(2) << x1 << " " << x3 << " " << x2 << endl;
    system("pause");
    return 0;
}