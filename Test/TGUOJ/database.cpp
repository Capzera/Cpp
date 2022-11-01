#include<bits/stdc++.h>
using namespace std;
class Student{
public:
    Student(string M,string N,string I,int S){
        major=M;
        name=N;
        id=I;
        score=S;
    }
    void print(){
        cout<<major<<" "<<id<<" "<<name<<" "<<score<<endl;        
    }
    ~Student(){};
private:
    string major,name,id;
    int score;
};
int main(){
    string Major="";
    vector<Student> stu;
    while (1)//输入#退出输入
    {
        int n;
        cout<<"请输入专业："<<endl;
        cin>>Major;
        if (Major[0]=='#') break;
        cout<<"请输入"<<Major<<"专业一共有几名学生："<<endl;
        cin>>n;
        for(int i=0;i<n;i++){
            cout<<"请输入第"<<i<<"名学生的信息："<<endl;
            string name,id;
            int score;
            cin>>id>>name>>score;
            Student tmp(Major,id,name,score);
            stu.push_back(tmp);
        }
    }
    for(auto i:stu){
        i.print();
    }
    system("pause");
    return 0;
}