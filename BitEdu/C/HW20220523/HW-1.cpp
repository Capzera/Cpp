#include<bits/stdc++.h>

using namespace std;


int main(){
    int bottom=20;
    int score=20;
    while (bottom>1) {
        score+=bottom/2;
        bottom/=2;
    }
    cout<<score<<endl;
    system("pause");
    return 0;
}