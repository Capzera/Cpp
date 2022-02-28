#include "menu.h"
using namespace std;
int main()
{   
    rgb_init();
    system("title 文件管理系统 V1.6");
    menu first;
    first.strin(s0,1);
    first.titlein("     文件管理系统");
    first.start(1);
    system("cls");
    cout<<"欢迎下次使用!"<<endl;
    system("pause");
    return 0;
}