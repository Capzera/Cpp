#include <bits/stdc++.h>
using namespace std;
bool isPara(string& s, int start, int end) {
    int n = end - start + 1;
    string str1 = s.substr(start, n / 2), str2 = s.substr(start + n / 2 + n % 2, n / 2);
    reverse(str2.begin(), str2.end());
    return str1 == str2;
}
int main() {
    cin.tie(nullptr)->sync_with_stdio(0);
    string str;
    cin >> str;
    int n = str.size();
    for (int len = 2; len <= n; len++)
        for (int i = 0; i <= n - len ; i++)
            if (isPara(str, i, i + len - 1)) cout << str.substr(i, len) << endl;
    system("pause");
    return 0;
}