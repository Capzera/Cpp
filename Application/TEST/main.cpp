#include"text.h"
#include<iostream>
using namespace std;
int main()
{
    Student student;
    student.namein("³ÂÆôº½");
    student.agein(19);
    student.scorein(150);
    cout<<student.whatname()<<" "<<student.whatage()<<" "<<student.whatscore()<<endl;
    system("pause");
    return 0;
}