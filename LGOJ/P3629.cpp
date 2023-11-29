#include <bits/stdc++.h>
using namespace std;

int check(int x) {
    int ans = x;
    while (x > 2) {
        ans += x / 3;
        x = x / 3 + x % 3;
    }
    return ans;
}

int main () {
    int n;
    cin >> n;
    int l = 0, r = n;
    while (l < r) {
        int mid = l + (r - l) / 2;
        if (check(mid) >= n) r = mid;
        else l = mid + 1; 
    }
    cout << r << endl;
    system("pause");
    return 0;
}
/*#include <bits/stdc++.h>
using namespace std;
int main() {
    int n;
    cin >> n;
    cout << 2 * n / 3 + 1 << endl;
    system("pause");
    return 0;
}
*/