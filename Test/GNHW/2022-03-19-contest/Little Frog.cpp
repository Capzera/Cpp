#include<bits/stdc++.h>
using namespace std;
int dfs(int n){
    if (n<3) return 1;
    return dfs(n-2)+dfs(n-3);
}
int main(){
    int n;
    cin>>n;
    cout<<dfs(n)<<endl;
    system("pause");
	return 0; 
}