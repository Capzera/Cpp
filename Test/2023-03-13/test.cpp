#include <bits/stdc++.h>
using namespace std;
int main() {
    vector<string> test = {"aa", "bb", "cc", "dd"};
    vector<string>::iterator iter = test.begin();
    cout << *(++iter) << endl;

    system("pause");
    return 0;
}