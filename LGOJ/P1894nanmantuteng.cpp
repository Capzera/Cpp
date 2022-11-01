#include <bits/stdc++.h>
using namespace std;
int main(){
	int n,wide=4;
	cin>>n;
    n=pow(2,n);//预处理n为2^n
    vector<string> ans(n,string(2*n,' '));//一共有2^n行，每一行有2^i个字符和2^i个空格，所以每一行的字符串长度预设为2^n+1个空格
	ans[0][0]=ans[1][1]='/';
	ans[0][1]=ans[0][2]='_';
	ans[0][3]=ans[1][2]='\\';
	while(wide<=n){//控制wide不能大于2^n
		for(int i=0;i<wide/2;i++){
			for(int j=0;j<wide;j++){
				ans[i][j+wide]=ans[i][j];//往右平移复制
				ans[i+wide/2][j+wide/2]=ans[i][j];//往下拓展复制
			}
		}
		wide*=2;
	}
	for(int i=n-1;i>=0;i--){//倒序输出，先打印尖角，再打印底座
		cout<<ans[i]<<endl;//每行都是用string保存的，直接输出整个字符串
	}
    system("pause");
	return 0;
}