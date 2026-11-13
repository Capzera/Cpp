#include<bits/stdc++.h>
using namespace std;
int a[9],sum=0;
bool b[9]={0},c[17]={0},d[17]={0};
void dfs(int);
void print(int);
int main(){
    dfs(1);
    system("pause");
    return 0;
}
void dfs(int i){
    int j,k;
    for(j=1;j<=8;j++)
        if (!b[j]&&!c[i+j]&&!d[i-j+8]){
            a[i]=j;
            b[j]=c[i+j]=d[i-j+8]=1;
            if(i==8){
                sum++;
                printf("            第%d种方案\n┌",sum);
                for(k=1;k<=8;k++)printf("───┐");
                for (k=1;k<=8;k++)print(k);
                printf("\n\n");
            }
            else dfs(i+1);
            b[j]=c[i+j]=d[i-j+8]=0;            
        }
}
void print(int t){
    int i1;
    printf("\n│");
    for(i1=1;i1<=8;i1++)
        if (i1==a[t]) printf(" %c │",31);
        else printf("   │");
    if (t<8){
        printf("\n├");
        for(i1=1;i1<=7;i1++)printf("───┼");
        printf("───┤");
    }
    else {
        printf("\n└");
        for(i1=1;i1<=7;i1++)printf("───┴");
        printf("───┘");
    }
}