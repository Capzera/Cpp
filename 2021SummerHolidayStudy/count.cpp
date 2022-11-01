#include<bits/stdc++.h>
using namespace std;
int main(){
    int k,a[21],x,l=0;
    scanf("%d",&k);
    for(int y=k+1;y<=2*k;y++){
        if (k*y%(y-k)==0){
            l++;
            x=(k*y)/(y-k);
            printf("1/%d = 1/%d + 1/%d\n",k,x,y);
        }
    }
    printf("%d\n",l);
    system("pause");
    return 0;
}
