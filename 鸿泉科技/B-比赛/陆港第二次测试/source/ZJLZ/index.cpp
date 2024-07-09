#include <iostream>
#include <vector>

using namespace std;

int main() {
    long long n;
    cin >> n;
    vector<int> a(n);

    for (int i = 0; i < n; ++i) {
        cin >> a[i];
    }

    long long mxVal = a[0];
    long long mxIdx = 1;

    for (int i = 1; i < n; ++i) {
        if (a[i] > mxVal) {
            mxVal = a[i];
            mxIdx = i + 1;
        } else if (a[i] == mxVal) {
            mxIdx = i + 1;
        }
    }

    cout << mxIdx << endl;
    return 0;
}
