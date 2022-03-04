#include "menu.h"
using namespace std;
int main()
{   
    rgb_init();
    system("title 文件管理系统 V1.7");
    menu first;
    first.strin(s0,1);
    first.titlein("     文件管理系统");
    first.start(1);
    system("cls");
    cout<<"欢迎下次使用!"<<endl;
    system("pause");
    return 0;
}
/*更新日志
V1.3
延展了蓝桥别竞赛题库下子集内容
V1.4
新增了菜单彩色效果
新增了MySQL、Application子菜单
V1.5
优化了算法，使得代码更轻便，减少内存消耗
V1.6
新增算法子菜单：双指针，哈希表，线段树，滑动窗口
V1.7
修复了LeetCode文件夹名的错误
新增了LeetCode子菜单:剑指Offer，热题100+
新增了算法子菜单：二分查找

*/