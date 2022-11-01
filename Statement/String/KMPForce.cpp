/*
    现给定一个文本串S，给定一个模式串P，求P在S中首个出现位置

    ABCDABCABCDABD ABCDABD
    7
*/
#include <bits/stdc++.h>

using namespace std;
vector<int> Next;

void Nextval(string p){
    int k = Next[0] = -1;
    int j = 0;
    while (j < p.size() - 1) {
        //p[k]表示前缀  p[j]表示后缀
        if (k == -1 || p[j] == p[k]) {// 如果没有前缀或当前匹配上了
            k++,j++;//k,j向右同时移动
            if (p[k] == p[j])//如果跳回k后还需要继续再跳，就再跳一次
                Next[j] = Next[k];
            else 
                Next[j] = k;//更新除第j位的最长公共前后缀
        }
        else
            k = Next[k];//更新前缀的新位置
    }
}

void show(){
    cout<<"Next:";
    for (auto& i:Next){
        cout<<i<<" ";
    }
    cout<<endl;
    system("pause");
    system("cls");
}

int KMP(string s, string p){
    int i = 0,j = 0;
    int slen = s.size(), plen = p.size();
    while (i < slen && j < plen){//匹配不完成时，i == slen结束，匹配完成时 j == plen
        if (j == -1 || s[i] == p[j]){//j == -1?
            i++,j++;
        }
        else {//(j!= -1 && s[i] != p[j])
            j = Next[j];
        }
    }
    if (j == plen) return i - j;
    else return -1;
}

int main(){
    string s,p;
    cin>>s>>p;
    Next = vector<int> (p.size());
    Nextval(p);
    show();
    cout<<"p在s的第"<<KMP(s,p)<<"位"<<endl;
    system("pause");
    return 0;
}
/*
原命题 否命题
逆命题 逆否命题
2>=1 || 5<2
2< 1 && 5 >=2
2<1 || 5<2
5<2 || 2>=1 
*/