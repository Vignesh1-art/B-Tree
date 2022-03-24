#include<iostream>
#include <stack>
#include <queue>
using namespace std;

class BNode{
    public:
    BNode(int size);
    int insertKey(int key);
    void print();
    int searchKey(int key);
    int getKey(int index);
    BNode *getChild(int index);
    void setChild(int index,BNode *child);
    bool isFull();
    int popBackKey();
    int popFrontKey();
    BNode *splitNode();
    bool leaf;
    int getSize();

    private:
    int size;
    int max_size;
    int *keys;
    BNode **children;
};

BNode::BNode(int t){
   size=0;
   max_size=t;
   keys=new int[t];
   children=new BNode*[t+1];
   for(int i=0;i<=t;i++){
        children[i]=nullptr;
   }
   leaf=true;
}
int BNode::getSize(){
    return size;
}
int BNode::insertKey(int key){
    if(size==max_size)//if full
        return -1;

    int i=size-1;
    while(i>=0 && keys[i]>key){
        keys[i+1]=keys[i];
        children[i+2]=children[i+1];
        i--;
    }
    i++;
    keys[i]=key;
    size++;
    return i;
}

void BNode::print(){
    for(int i=0;i<size;i++){
        if(i!=0)
            cout<<" ";
        cout<<keys[i];
    }
}

void BNode::setChild(int index,BNode* child){
    children[index]=child;
}

bool BNode::isFull(){
    return size==max_size;
}

int BNode::searchKey(int key){
    int i=0;
    while(i<size && key>keys[i])
        i++;
    return i;
}

int BNode::popBackKey(){
    if(size==0)
        return -1;
    int temp=keys[size-1];
    size--;
    return temp;
}

int BNode::popFrontKey(){
    if(size==0)
        return -1;
    int temp=keys[0];
    for(int i=1;i<size;i++){
        keys[i-1]=keys[i];
        children[i-1]=children[i];
    }
    children[size-1]=children[size];
    size--;
    return temp;
}

int BNode::getKey(int index){
    return keys[index];
}

BNode *BNode::getChild(int index){
    return children[index];
}

BNode *BNode::splitNode(){
    int h=size/2;
    int offset=size%2;
    BNode *x=new BNode(size);
    x->leaf=leaf;
    for(int i=0;i<h;i++){
        x->keys[i]=keys[h+i+offset];
        x->children[i]=children[h+i+offset];
    }
    x->children[h]=children[size];
    size=size-h;
    x->size=h;
    return x;
}
///-------------------------------------------////
class BTree{
    BNode *root=nullptr;
    int t;
    int *keyBuffer;

    public:
    BTree(int nodesize);
    void insertKey(int key);
    void print();

};

BTree::BTree(int nodesize){
    t=nodesize;
    root=new BNode(t);
    keyBuffer=new int[t+1];
}

void BTree::print(){
    queue<BNode *> nodeQueue;
    nodeQueue.push(root);
    int cur_lvl_count=1;
    while(!nodeQueue.empty()){

        for(int i=0;i<cur_lvl_count;i++){
            BNode *cur=nodeQueue.front();
            nodeQueue.pop();
            cout<<"|";
            cur->print();
            cout<<"|";
            int n=cur->getSize();
            for(int j=0;j<=n;j++){
                BNode *temp=cur->getChild(j);
                if(temp)
                    nodeQueue.push(temp);
            }
        }
        cout<<endl;
        cout<<endl;
        cur_lvl_count=nodeQueue.size();


    }

}

void BTree::insertKey(int key){
    BNode *cur=root;
    stack<BNode *> nodeStack;

    while(!cur->leaf){
        int index=cur->searchKey(key);
        if(cur->getKey(index)==key){
            //if same key found
            return;
        }
        BNode *child=cur->getChild(index);
        nodeStack.push(cur);
        cur=child;
    }

    BNode *rchild=nullptr,*lchild=nullptr;
    while(true){
        if(cur->isFull()){
            BNode *temp=cur->splitNode();
            temp->setChild(0,rchild);
            int cur_end_key=cur->getKey(cur->getSize()-1);
            int temp_first_key=temp->getKey(0);
            int mid;

            if(key>cur_end_key && key<temp_first_key){
                mid=key;
            }
            else if(temp_first_key<key){
                mid=temp->popFrontKey();
                int index=temp->insertKey(key);
                temp->setChild(index,lchild);
                temp->setChild(index+1,rchild);
            }
            else{
                mid=cur->popBackKey();
                int index=cur->insertKey(key);
                cur->setChild(index,lchild);
                cur->setChild(index+1,rchild);
            }

            if(cur==root){
                root=new BNode(t);
                root->insertKey(mid);
                root->setChild(0,cur);
                root->setChild(1,temp);
                root->leaf=false;
                break;
            }
            lchild=cur;
            rchild=temp;
            cur=nodeStack.top();
            nodeStack.pop();
            key=mid;

        }
        else{
            int index=cur->insertKey(key);
            cur->setChild(index,lchild);
            cur->setChild(index+1,rchild);
            break;
        }

    }


}


int main(){
    BTree *tree=new BTree(2);
    tree->insertKey(4);
    tree->insertKey(40);
    tree->insertKey(30);
    tree->insertKey(50);
    tree->insertKey(60);
    tree->insertKey(70);
    tree->insertKey(80);
    tree->insertKey(1);
    tree->insertKey(2);
    tree->insertKey(3);
    tree->insertKey(5);
    tree->insertKey(110);
    tree->insertKey(111);
    tree->insertKey(112);
    tree->insertKey(113);
    tree->print();
    return 0;
}
