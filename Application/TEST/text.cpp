#include "text.h"
void Student::namein(string s)
{
    this->name=s;
}
void Student::agein(int n)
{
    this->age=n;
}
void Student::scorein(int n)
{
    this->score=n;
}
string Student::whatname()
{
    return name;
}
int Student::whatage()
{
    return age;
}
int Student::whatscore()
{
    return score;
}