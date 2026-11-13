#include <bits/stdc++.h>
using namespace std;
int main() {
    int n = 2415229;
    vector<long long> prime, st(n + 21);
    for (int i = 2; i <= n + 20; i++) {
        if (!st[i]) prime.push_back(i);
        for (int j = 0; j < prime.size() && prime[j] * i <= n; j++)
            st[prime[j] * i] = 1;
    }
    long long ans = 0;
    for (int i = 0; i < prime.size() - 1; i++) {
        int edge = i + 1;
        while (1LL * prime[i] * prime[edge] < 48) edge++;
        int l = edge, r = prime.size();
        while (l < r) {
            int mid = (l + r) / 2;
            if (1LL * prime[mid] * prime[i] > 1LL * 2 * n) r = mid;
            else l = mid + 1;
        }
        ans += l - edge;
    }
    cout << ans << endl;
    system("pause");
    return 0;
}
/*
(prime[i] * q)^ 2 < 2 * n
2 3 5 7 11 13 17 19 23 29
11 13 17 19 23 29
90 r = 17
l = 17 r = 17
i = 5
edge = 11
prime[i] * x > 48

q < 2 * n / prime[i];

*/