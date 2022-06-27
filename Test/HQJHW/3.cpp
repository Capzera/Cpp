#include <bits/stdc++.h>

using namespace std;

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(NULL), right(NULL) {};
    TreeNode(int x,TreeNode* l,TreeNode* r):val(x),left(l),right(r) {};
};

class BST {
public:
    void Init();
    void Input();
    void Insert(int);
    void Show(TreeNode*);
    void Find();
private:
	TreeNode* root;
};

void BST::Init(){
    
}

void BST::Insert(int val){

}

void BST::Input(){
    int n;
    cout<<"请输入输入数据个数：";
    cin>>n;
    cout<<"请输入数据，以空格分割:"<<endl;
    for (int i=0;i<n;i++){
        int put;
        cin>>put;
        Insert(put);
    }
    cout<<"BST为：";
    Show(root);
}

void BST::Show(TreeNode* root){
    if (!root) return;
    Show(root->left);
    cout<<root->val<<" ";
    Show(root->right);
}

void BST::Find(){
    
}

int main(){
    BST bst;
    bst.Init()
    system("pause");
    return 0;
}