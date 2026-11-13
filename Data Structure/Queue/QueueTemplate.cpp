#include<bits/stdc++.h>

using namespace std;

struct Node{
    int val;
    Node* next;
    Node():val(0),next(nullptr){};
    Node(int x):val(x),next(nullptr){};
    Node(int x,Node* p):val(x),next(p){};
};

class Queue{
public:
    Queue(){};
    void Init();
    void Input();
    void Push(int);
    void Pop();
    void Show();
    ~Queue(){};
private:
    Node* front;
    Node* rear;
};

void Queue::Init(){
    Node *p=new Node(-1);
    front=p;
    rear=p;
}

void Queue::Input(){
    int n;
    cout<<"请输入输入数据个数：";
    cin>>n;
    cout<<"请输入数据，以空格分割:"<<endl;
    for (int i=0;i<n;i++){
        int put;
        cin>>put;
        Push(put);
    }
    cout<<"已插入，队列为：";
    Show();
}

void Queue::Push(int x){
    Node *p = new Node(x);
    rear->next=p;
    rear=p;
}

void Queue::Pop(){
    Node *p=front->next;
    front->next=front->next->next;     
    if (rear==p) rear=front;
    delete p;
}

void Queue::Show(){
    Node *p=front->next;
    if (!p) cout<<"队列为空"<<endl;
    while (p) {
        cout<<p->val<< " ";
        p=p->next;
    }
    cout<<endl;
}

int main(){
    Queue q;
    q.Init();
    q.Input();
    q.Push(5);
    q.Show();
    q.Pop();
    q.Show();
    system("pause");
    return 0;
}