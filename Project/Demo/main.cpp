#include <bits/stdc++.h>

using namespace std;

int main()
{
    int a,b;
    cin>>a>>b;
    if (a>b) {
        cout<<a*a<<endl;
    }
    else if (a<b){
        cout<<b*a<<endl;
    }
    else cout<<b*b<<endl;

    return 0;
}
