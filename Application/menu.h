#pragma once
#include<bits/stdc++.h>
#include "color.h"
#include<conio.h>
#include<fstream>
using namespace std;
string DAD="D:\\Program\\C++\\";
static vector<string>
    s0=
    {
        {"2021暑假集训"},
        {"算法"},
        {"应用"},
        {"蓝桥杯竞赛"},
        {"数据结构"},
        {"LeeCode"},
        {"洛谷OJ"},
        {"数据库MySQL"},
        {"工程"},
        {"Python"},
        {"基础C++语言部分"},
        {"测试"}
    },
    s1=
    {
        {"深度优先搜索"},
        {"分治"},
        {"双指针"},
        {"动态规划"},
        {"贪心"},
        {"哈希表"},
        {"高精度"},
        {"递归"},
        {"递推"},
        {"搜索与回溯"},
        {"滑动窗口"},
        {"排序"}
    },
    s2=
    {
        {"2013年试题"},
        {"2021西安工程大学校赛"},
        {"官网OJ"}
    },
    s3=
    {
        {"数组与广义表"},
        {"线性表"},
        {"链表"},
        {"队列"},
        {"排序"},
        {"栈"},
        {"树"},
    },
    s4=
    {
        {"数组"},
        {"循环结构"},
        {"类与对象"},
        {"指针"},
        {"选择结构"},
        {"顺序结构"},
        {"字符串"},
        {"结构体"},
    },
    s5=
    {
        {"ALGO"},
        {"BASIC"},
        {"PreTest"},
    };
class menu
{
public:
    menu(){}
    void strin(vector<string>,int);
    void printmenustr();
    void start(int);
    void select(int,int);
    void textcreate(string str);
    int listsize(vector<string>);
    ~menu(){}
private:
    vector<string> s;
    string title="     文件管理系统";
};
int menu::listsize(vector<string> t)
{
    return t.size();
}
void menu::strin(vector<string> s,int x)
{
    int n=s.size();
    for(int i=0;i<n;i++) this->s.push_back(s[i]);
    if (x==1)this->s.push_back("退出");
    else this->s.push_back("返回");
}
void menu::printmenustr()
{
    int i,n=s.size();
    char c;
    for(i=0;i<n;i++) 
    {
        int r=rand()%255,g=rand()%255,b=rand()%255;
        rgb_set(r,g,b);
        c=i+97;
        cout<<c<<"."<<s[i]<<endl;
    }
}
void menu::start(int x)
{
    system("cls");
    int n=listsize(s);
    char ch='0';
    while (ch!=n+96) 
    {
        system("cls");
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY| FOREGROUND_GREEN);
        cout<<title<<endl;
        printf("\\-----By:机长大大-----/\n"); 
        printf(" \\--------V1.2-------/\n");
        printmenustr();
        if (ch-96>0&&ch-96<n) 
        {
            select(ch-96,x);
            ch='0';
        }
        else 
        {
            ch=getch();
            if (ch>64&&ch<91) ch+=32;
        }
    }
}
void menu::select(int x,int n)//x控制进到哪个子分类，n控制第几级菜单
{
    menu a,b,c,d,e;
    a.strin(s1,2);
    a.title="        算法";
    b.strin(s2,2);
    b.title="      蓝桥杯";
    c.strin(s3,2);
    c.title="       数据结构";
    d.strin(s4,2);
    d.title="      基础语言";
    e.strin(s5,2);
    e.title="    蓝桥杯题库";
    if (n==1)
    {
        switch(x)
        {
            case 1:
                textcreate(DAD+"2021SummerHolidayStudy\\P");
            break;
            case 2:
                a.start(2);
            break;
            case 3:
                textcreate(DAD+"Application\\P");
            break;
            case 4:
                b.start(3);
            break;
            case 5:
                c.start(4);
            break;
            case 6:
                textcreate(DAD+"LeeCode\\P");    
            break;
            case 7:
                textcreate(DAD+"LGOJ\\P");
            break;
            case 8:
                textcreate(DAD+"MySQL\\P");
            break;
            case 9:
                textcreate(DAD+"Project\\P");
            break;
            case 10:
                textcreate(DAD+"Python\\P");
            break;
            case 11:
                d.start(5);
            break;
            case 12:
                textcreate(DAD+"Test\\P");
            break;
        }
    }
    if (n==2)
    {
        switch(x)
        {
            case 1:
                textcreate(DAD+"Algorithm\\Breadth First Search Algorithm\\LCP");
            break;
            case 2:
                textcreate(DAD+"Algorithm\\Divide and Conquer Algorithm\\LCP");
            break;
            case 3:
                textcreate(DAD+"Algorithm\\Double Pointer\\LCP");
            break;
            case 4:
                textcreate(DAD+"Algorithm\\Dynamic Programming\\LCP");
            break;
            case 5:
                textcreate(DAD+"Algorithm\\Greedy Algorithm\\LCP");
            break;
                textcreate(DAD+"Algorithm\\HashMap\\LCP");  
            break;
            case 7:
                textcreate(DAD+"Algorithm\\HighAccuracy Algorithm\\LCP");
            break;
            case 8:
                textcreate(DAD+"Algorithm\\Recursion Algorithm\\LCP");
            break;
            case 9:
                textcreate(DAD+"Algorithm\\Recursive Algorithm\\LCP");
            break;
            case 10:
                textcreate(DAD+"Algorithm\\Search and Backtracking Algorithm\\LCP");
            break;
            case 11:
                textcreate(DAD+"Algorithm\\Sliding Window\\LCP");
            break;
            case 12:
                textcreate(DAD+"Algorithm\\Sorting Algorithm\\LCP");
            break;
        }
    }
    if (n==3)
    {
        switch(x)
        {
            case 1:
                textcreate(DAD+"BlueBridgeCompetitive\\2013\\P");
            break;
            case 2:
                textcreate(DAD+"BlueBridgeCompetitive\\2021BlueBridgeXPUCompetitive\\P");
            break;
            case 3:
                e.start(6);
            break;
        }
    }
    if (n==4)
    {
        switch(x)
        {
            case 1:
                textcreate(DAD+"Data Structure\\Array and Generalized\\LCP");
            break;
            case 2:
                textcreate(DAD+"Data Structure\\Linear List\\LCP");
            break;
            case 3:
                textcreate(DAD+"Data Structure\\LinkList\\LCP");
            break;
            case 4:
                textcreate(DAD+"Data Structure\\Queue\\LCP");
            break;
            case 5:
                textcreate(DAD+"Data Structure\\Sort\\LCP");
            break;
            case 6:
                textcreate(DAD+"Data Structure\\Stack\\LCP");
            break;
            case 7:
                textcreate(DAD+"Data Structure\\Tree\\LCP");
            break;            
        }
    }
    if (n==5)
    {
        switch(x)
        {
            case 1:
                textcreate(DAD+"Statement\\Array\\LCP");
            break;
            case 2:
                textcreate(DAD+"Statement\\Loop Oganization\\LCP");
            break;
            case 3:
                textcreate(DAD+"Statement\\Objects\\LCP");
            break;
            case 4:
                textcreate(DAD+"Statement\\Pointer\\LCP");
            break;
            case 5:
                textcreate(DAD+"Statement\\Selection Oganization\\LCP");
            break;
            case 6:
                textcreate(DAD+"Statement\\Sequential Oganization\\LCP");
            break;
            case 7:
                textcreate(DAD+"Statement\\String\\LCP");
            break;
            case 8:
                textcreate(DAD+"Statement\\Structre\\LCP");
            break;           
        }
    }
    if (n==6)
    {
        switch(x)
        {
            case 1:
                textcreate(DAD+"BlueBridgeCompetitive\\OpenJudge\\ALGO\\ALGO-");
            break;
            case 2:
                textcreate(DAD+"BlueBridgeCompetitive\\OpenJudge\\BASIC\\BASIC-");
            break;
            case 3:
                textcreate(DAD+"BlueBridgeCompetitive\\OpenJudge\\PreTest\\PT-");
            break;
        }
    }
}
void menu::textcreate(string address)
{
    fstream File;
    system("cls");
    string str="",swi="";
    int number,times=-1;
    cout<<"请输入题目序号:";
    cin>>number;
    if (number<0) return;
    if (number<10) times=3;
    else if (number<100) times=2;
    else if (number<1000) times=1;
    for(int i=0;i<times;i++)str.push_back('0');
    swi=to_string(number);
    str+=swi;
    File.open(address+str+".cpp",ios::out);
            File<<"/*"<<endl<<"问题描述：P"<<str<<endl<<endl<<"*/"<<endl;
    File.close();
    system("cls");
    cout<<"P"<<str<<".cpp"<<"创建完毕！"<<endl;
    system("pause");
}