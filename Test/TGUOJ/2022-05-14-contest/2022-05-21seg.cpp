#include<bits/stdc++.h>
using namespace std;
vector<int> nums,location;
vector<bool> ind;
int n,k;
void dfs(int cur) {
    if (cur==k) {//如果已经选取了k个数
        for (auto i:location) cout<<i<<" ";//将保存的答案输出
        cout<<endl;
        return;//不再继续寻找下一个数，回溯回去
    }
    for (int i=0;i<n;i++) {
        if (ind[i]) continue;//如果这个数被选取了，就跳过
        ind[i]=1;//选取这个数
        location[cur]=nums[i];//保存这个数到底是哪个
        dfs(cur+1);//开始找下一个数
        ind[i]=0;//回溯回来之后，取消这个数的标记
    }
}
int main(){
    cin>>n>>k;
    nums=vector<int> (n);//nums存放n个数
    location=vector<int> (k);//location存放答案
    ind=vector<bool>(n,false);//标记数组，代表每个数是否被选取
    for (int i=0;i<n;i++) cin>>nums[i];//读入
    dfs(0);
    system("pause");
    return 0;
}