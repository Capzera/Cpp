#include <iostream>

using namespace std;

class Test {
public:
    friend ostream& operator << (ostream& out,const Test& t) {
        out << t.m_data << endl;
        return out;
    }
    Test operator + (Test& t) {
       Test tmp(this->m_data + t.m_data);
       return tmp;
    }
    Test(int data = 0) :m_data(data){}
private:
    int m_data;
};
int main() {
    Test a(1), b(2);
    cout << a + b << endl;
    return 0;
}
/*
    左值  右值
    const：常引用 非常引用
    1.常左值引用
    2.非常左值引用
    3.常右值引用
    4.非常右值引用
    非常左值引用能对绑定对象修改，但是非常左值引用只能绑定非常左值
    常左值引用为万能引用，能够与以上4种任何一个绑定引用
    int a = 1 + 2   a是左值 1 + 2 是右值
    Test& t 非常左值引用
    cout << c << endl;  c是左值
    cout << a + b << endl; a + b右值
    a + b属于非常右值
    int a, b
    cin >> a >> b
    c = a+ b;
    cout << a + b << endl;
    dfs(i + 1)
    sort([](int& a, int& b)

    996 2 14 13 4 2 // +
    14 // 2 = 7
    7 << 2 = 28
    28 + 996 = 1024
*/
