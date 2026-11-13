#include<bits/stdc++.h>

using namespace std;

struct ListNode {
    int val;
    ListNode* next;
    ListNode():val(0),next(nullptr){};
    ListNode(int x):val(x),next(nullptr){};
    ListNode(int x,ListNode* p):val(x),next(p){};
};

class Node {
public:
    Node(){};
    void Init();//调用建立空链表
    void Input();//调用向链表中插值
    void Insert(int);//插值函数
    void Sort();//调用则插入排序链表
    void Delete();//调用删除链表中的值
    void Show();//输出链表
    ~Node(){};
private:
    ListNode* head;
    ListNode* p;
};

void Node::Init() {
    head=new ListNode(-1);
    p=head;
}

void Node::Insert(int x) {
    ListNode *q=new ListNode(x);
    p->next=q;
    p=p->next;//尾插
}

void Node::Input() {
    int n;
    cout<<"请输入输入数据个数：";
    cin>>n;
    cout<<"请输入数据，以空格分割:"<<endl;
    for (int i=0;i<n;i++){
        int put;
        cin>>put;
        Insert(put);
    }
    cout<<"已插入，链表为：";
    Show();
}

void Node::Show() {
    p=head;
    cout<<"链表打印为：";
    while (p->next){//遍历链表
        p=p->next;
        cout<<p->val<<" ";
    }
    cout<<endl;
}

void Node::Sort() {
    p=head;
    ListNode* newHead=new ListNode(INT_MIN);
    ListNode* p1;
    while (p->next){
        p=p->next;//找到待插入的值
        ListNode* q=new ListNode(p->val);
        p1=newHead;
        while (p1->next&&p1->next->val<p->val) {
            p1=p1->next;
        }//确定待插入的位置
        if (p1->next) {//如果是中间插值
            q->next=p1->next;
            p1->next=q;
        }
        else p1->next=q;//如果是尾插
    }
    head=newHead;
    cout<<"已排序，新链表为：";
    Show();
}

void Node::Delete(){
    int x;
    cout<<"请输入要删除的值：";
    cin>>x;
    p=head;
    while (p->next&&p->next->val<x){
        p=p->next;
    }//找到待删除值的前一个
    if (p->next==nullptr||p->next->val>x){//如果最后一个值小于x或下一个值大于x，x不在链表中
        cout<<"该值不存在！"<<endl;
        return;
    }
    p->next=p->next->next;
    cout<<"已删除，新链表为：";
    Show();
}

int main() {
    Node LN;
    LN.Init();
    LN.Input();
    LN.Sort();
    LN.Delete();
    system("pause");
    return 0;
}