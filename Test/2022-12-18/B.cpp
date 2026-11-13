#include <bits/stdc++.h>
using namespace std;
int main() {
    string str;
    cin >> str;
    int n = str.size(), i = 0, j = 1;
    while (j < n) {
        while (j < n && str[j] == '@') j++;
        if (j == n) break;
        while (i < j && str[i] != '@') i++;
        swap(str[i++], str[j++]);
        
    }
    cout << str << endl;
    system("pause");
    return 0;
}
/*
@@@@ab@CD@G@@@
ab@CD@G@@@@@@@
*/