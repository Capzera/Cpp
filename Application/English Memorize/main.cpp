#include<bits/stdc++.h>
#include<conio.h>
using namespace std;
int main(){
    vector<pair<string,char>> um=//第一单元
    {
        {"should take",'A'},
        {"For some",'B'},
        {"You can bounce",'B'},
        {"The most devastating",'C'},
        {"It's necessary",'C'},
        {"You should find",'E'},
        {"Not only",'F'},
        {"The backup",'F'},
        {"The quality",'G'},
        {"You should prepare",'H'},
    };
    /*vector<pair<string,char>> um=//第二单元
    {
        {"Extremely",'G'},
        {"Children in",'B'},
        {"The author suggest",'E'},
        {"Men seek",'A'},
        {"Children short",'D'},
        {"The five suggestions",'I'},
        {"Doctor William",'F'},
        {"Elderly",'E'},
        {"The author believe",'H'},
        {"A series",'C'},
    };*/
    /*vector<pair<string,char>> um=//第四单元
    {
        {"Many",'D'},
        {"A century ago",'B'},
        {"The interstate",'I'},
        {"General Eisenhower",'B'},
        {"It was",'D'},
        {"Under",'F'},
        {"Trucks",'H'},
        {"Thanks",'G'},
        {"To",'H'},
        {"In",'E'},
    };*/
    /*vector<pair<string,char>> um=//第五单元
    {
        {"To address",'I'},
        {"Liquid",'F'},
        {"In order to",'D'},
        {"Excessive sugar",'C'},
        {"In the author",'G'},
        {"Education",'I'},
        {"Americans",'B'},
        {"A gentle way",'J'},
        {"Some researchers",'A'},
        {"The regulation of",'H'},
    };*/
    system("title 机长大大英语期末考试背书软件V1.2");
    vector<bool> check(10);
    vector<string> wrong;
    int n=10,r=11;
    char put;
    double score=0;
    srand((unsigned)time(NULL));
    while (n>0)
    {
        system("cls");
        while (r>10||check[r]) r=rand()%10;
        check[r]=1;
        cout<<um[r].first<<endl;
        put=getch();
        if (put>96) put-=32;
        if (put==um[r].second) score++;
        else wrong.push_back(um[r].first);
        n--;
    }
    system("cls");
    score*=10;
    cout<<"正确率："<<score<<"%"<<endl;
    if (wrong.size()) cout<<"一共错误了"<<wrong.size()<<"个题目，分别是："<<endl;
    else cout<<"恭喜！全对！"<<endl;
    for(int i=0;i<wrong.size();i++) cout<<wrong[i]<<endl;
    system("pause");
    return 0;
}