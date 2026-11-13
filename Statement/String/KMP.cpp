#include<bits/stdc++.h>
using namespace std;
class KMP{
public:
    KMP(string s,string t):s(s),t(t),len1(s.size()),len2(t.size()){
	    GetNextval(t);
    }
    void GetNextval(string t)//获取nextval数组
    {
        nextval=vector<int>(t.size());//定义一个长度为t-1的数组
	    nextval[0]=-1;//定义初始边界
	    for (int i=1,j=0;i<len2;i++) {//遍历t字符串，从1开始，到t-2结束
            if (j==-1||t[i]==t[j]) {//j==-1说明没有公共前后缀，t[i]==t[j]当前两个指针指向的量相同
                j++;
                if (t[i]!=t[j]) nextval[i]=j;//nextval直接推对应在前缀后面的下标
                else nextval[i]=nextval[j];//nextval直接往右拉
            }
            else j=nextval[j];//需要回溯
	    }
    } 
    int KMPindex(string s,string t) {//求KMP算法
        int i=0,j=0;
	    while (i<len1&&j<len2) {
		    if (j==-1||s[i]==t[j]) {
                i++;
                j++;
            }
		    else j=nextval[j];
        }
        if (j>=len2) return(i-len2);
        else  return(-1);
    }
    void getNextArray(){
        for (auto& i:nextval){
            cout<<i<<" ";
        }
        cout<<endl;
        system("pause");
        system("cls");
    }
    void show(){//打印函数
        int val=KMPindex(s,t);
        if (val<0) cout<<"未匹配"<<endl;
        else cout<<"匹配索引为"<<val<<"--"<<val+len2-1<<endl;
    }
private:
    string s,t;
    int len1,len2;
    vector<int> nextval;
};
int main(){
    string s1,s2;
    cin>>s1>>s2;
    KMP K(s1,s2);
    K.getNextArray();
    K.show();
    system("pause");
    return 0;
}