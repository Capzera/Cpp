#include<bits/stdc++.h>
using namespace std;
void sort(char *str[],int n);
void print(char *str[],int n);
int main(){
    char *str[101];
    int n,i;
    cin>>n;
    for(i=0;i<n;i++){
        gets(str);
    }
    sort(str,n);
    print(str,n);
    system("pause");
    return 0;
}
void sort(char *str[],int n){
    int i,j;
    char *temp;
    for(i=0;i<n-1;i++)
        for(j=i+1;j<n;j++)
            if(strcmp(str[i],str[j])>0){
                temp=str[i];
                str[i]=str[j];
                str[j]=temp;
            }
}
void print(char *str[],int n){
    int i;
    for(i=0;i<n;i++)cout<<str[i]<<endl;
}