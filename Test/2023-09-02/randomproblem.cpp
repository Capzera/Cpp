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
        "JEE ��,HtttpServletRequest���( )�������ڴ����Ի�",
        "����( )�����ڼ���session����userid��ֵ",
        "�������ORM������ȷ����( )",
        "����AOP������ȷ����( )",
        "AOP����������,��Ҫ������������,��ȷ����( )",
        "���·�����,( )����HttpServlet��ķ���",
        "Tomcat��Ĭ�϶˿���( )",
        "���в���JSP�������ͨ�Ŷ������( )"
    }, sa = {
        "ʹ����ṹ������",
        "�������ע��",
        "getSession ",
        "session. getAttribute (��userid��)",
        "ORM����ͨ�����������ֶβ����κ��������ݿ�",
        "AOP�����õ����ģʽ�Ǿ�̬����",
        "���棬���ܣ������߼�",
        "doGet",
        "7001",
        "session"
    }, sb = {
        "ʹ�������ά��",
        "ͨ���κη�������ע��",
        "newSession",
        "session. setAttribute (��userid��)",
        "ORM��hibernate���е�һ�����˼��",
        "Java�ķ��������AOP��û���õ�",
        "���棬֯�룬�����߼�",
        "service",
        "8080",
        "application"
    }, sc = {
        "��֤��ͼ��ģ�͵ĸ���",
        "ͨ��set����ע������",
        "newInstance",
        "request. getParameter (��userid��)",
        "ORMר���ڲ��������������ݿ�",
        "��̬������ʵ��AOP�����ģʽ",
        "���棬����㣬�����߼�",
        "doPost",
        "80",
        "pageContext"
    }, sd = {
        "���߼���ʵ����ͼ��ģ�ͺͿ������������κι�ϵ",
        "��ͨ���κη�����ֱ��ע������ֵ",
        "getAttribute",
        "request. getAttribute (��userid��)",
        "ORM��ͨ���������ķ�ʽ������ϵ�����ݿ�",
        "���϶���",
        "���϶���",
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
            cout << "��ȷ��" << endl;
            cor++;
        }
        else {
            cout << "����" << endl;
        }
        system("pause");
        system("cls");
    }
    double x = 100.0 * cor / tot;
    printf("��ȷ��Ϊ%.2f", x);
    cout << "%";
    system("pause");
    return 0;
}