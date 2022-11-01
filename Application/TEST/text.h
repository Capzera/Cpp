#include<bits/stdc++.h>
using namespace std;
class Student
{
public:
    Student(){}
    void namein(string);
    void agein(int);
    void scorein(int);
    string whatname();
    int whatage();
    int whatscore();
    ~Student(){}
private:
    string name;
    int age;
    int score;
};