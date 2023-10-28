#include <iostream>
#include <stdio.h>
#include <vector>
#include <stdlib.h>
#include <ctime>
#include <unordered_set>
#include <string>
using namespace std;
int main() {
    int cor = 0, tot = 0;
    srand(time(NULL));
    unordered_set<int> ps;
    vector<string> pro = {
        "以下哪项不是MVC设计模式的目的( )",
        "下面那种类型不是Spring所支持的依赖注入类型( )",
        "JEE 中，HtttpServletRequest类的( )方法用于创建对话",
        "以下( )可用于检索session属性userid的值",
        "下面关于ORM描述正确的是( )",
        "对于AOP描述正确的是( )",
        "AOP面向切面编程，主要包括三个方面，正确的是( )",
        "以下方法中，哪个不是HttpServlet类的方法( )",
        "Tomcat的默认端口是( )",
        "下列不是JSP作用域的通信对象的有( )"
    }, sa = {
        "使程序结构更清晰",
        "构造参数注入",
        "getSession ",
        "session. getAttribute (\"userid\")",
        "ORM就是通过面向对象的手段操作任何数据数据库",
        "AOP所采用的设计模式是静态代理",
        "切面，功能，切入逻辑",
        "doGet",
        "7001",
        "session"
    }, sb = {
        "使程序更好维护",
        "通过任何方法参数注入",
        "newSession",
        "session. setAttribute (\"userid\")",
        "ORM是hibernate独有的一种设计思想",
        "Java的反射机制在AOP中没有用到",
        "切面，织入，切入逻辑",
        "service",
        "8080",
        "application"
    }, sc = {
        "保证视图和模型的隔离",
        "通过set方法注入属性",
        "newInstance",
        "request. getParameter (\"userid\")",
        "ORM专用于操作面向对象的数据库",
        "动态代理是实现AOP的设计模式",
        "切面，切入点，切入逻辑",
        "doPost",
        "80",
        "pageContext"
    }, sd = {
        "在逻辑上实现视图、模型和控制器三者无任何关系",
        "不通过任何方法，直接注入属性值",
        "getAttribute",
        "request. getAttribute (\"userid\")",
        "ORM是通过面向对象的方式操作关系型数据库",
        "以上都对",
        "以上都对",
        "doDelete",
        "112",
        "out"
    };
    vector<char> ans = {
        'D',
        'D',
        'B',
        'A',
        'D',
        'C',
        'C',
        'B',
        'B',
        'D'
    };
    char ch = 'z';
    auto check = [&](string cur, int id) {
        string str;
        if (ans[id] == 'A') str = sa[id];
        if (ans[id] == 'B') str = sb[id];
        if (ans[id] == 'C') str = sc[id];
        if (ans[id] == 'D') str = sd[id];
        return cur == str;
    };
    while (1) {
        int id = rand() % 10, ran;
        if (ps.size() == 10) ps.clear();
        while (ps.count(id)) {
            id = rand() % 10;
        }
        ps.emplace(id);
        cout << pro[id] << endl;
        unordered_set<int> s;
        while (s.size() < 4) {
            ran = rand() % 4;
            s.emplace(ran);
        }
        char num = 'A', an = 'z';
        for (auto & k : s) {
            cout << num << " ";
            if (k == 0) {
                cout << sa[id] << endl;
                if (check(sa[id], id)) an = num;
            }
            if (k == 1) {
                cout << sb[id] << endl;
                if (check(sb[id], id)) an = num;
            }
            if (k == 2) {
                cout << sc[id] << endl;
                if (check(sc[id], id)) an = num;
            }
            if (k == 3) {
                cout << sd[id] << endl;
                if (check(sd[id], id)) an = num;
            }

            num++;
        }
        cin >> ch;
        ch = toupper(ch);
        if (ch == 'Q') break;
        tot++;
        if (an == ch) {
            cout << "正确" << endl;
            cor++;
        }
        else {
            cout << "错误" << endl;
        }
        system("pause");
        system("cls");
    }
    system("cls");
    cout << "共答 " << tot << " 题， ";
    double x = 100.0 * cor / tot;
    printf("正确率： %.2f", x);
    cout << "%" << endl;
    system("pause");
    return 0;
}
