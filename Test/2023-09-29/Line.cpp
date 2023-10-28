#include <iostream>
#include <math.h>
#include <algorithm>
using namespace std;
struct line {
    float k, b;
}l[200000];
int main() {
    int n = 0;
    for (int x1 = 0; x1 < 20; x1++) {
        for (int y1 = 0; y1 < 21; y1++) {
            for (int x2 = 0; x2 < 20; x2++) {
                for (int y2 = 0; y2 < 21; y2++) {
                    if (x1 != x2) {
                        double k = (double) (y2 - y1) / (x2 - x1);
                        double b = y1 - k * x1;
                        l[n].k = k, l[n].b = b;
                        n++;
                    }
                }
            }
        }
    }
    sort(l, --n);
    int ans = 1;
    for (int i = 1; i < n; i++) {
        if (fabs(l[i].k - l[i - 1].k) > 1e-8 || fabs(l[i].b - l[i - 1].b) > 1e-8) {
            ans++;
        }
    }
    printf("%d", ans + 20);
    return 0;
}