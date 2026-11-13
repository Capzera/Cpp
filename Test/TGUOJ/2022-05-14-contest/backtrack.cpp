#include<bits/stdc++.h>
using namespace std;
void dfs(int 当前是哪一层){
    if (最后一层){
        记录答案/答案+1/输出答案;
        return;
    }
    for (当前这层所有的情况){
        if (被访问过) continue;
        *做标记/记录;
        dfs(下一层);
        *取消标记;
    }
}
int main(){
    dfs(0);
}