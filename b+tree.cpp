#include<bits/stdc++.h>
using namespace std;

class BtreeNode{
    public:
        BtreeNode(int,int);
        virtual ~BtreeNode();

        int getsize();
        int* getkeys();
        BtreeNode** getptr();
        int gett();
        int is_leaf();

        void setkeys(int,int);
        void size_increase();
        void setSize(int);
        void setptr(int,BtreeNode*);
        bool insert_value(int);
        void empty();
        void setleaf(bool);
    private:
        int t;
        int size;
        int leaf;
        int *keys;
        BtreeNode** ptr;
};

BtreeNode::BtreeNode(int t_value,int value){
    t = t_value;
    size = 0;
    if(value == 0){
        leaf = false;
        keys = new int[2*t+1];
        ptr = new BtreeNode*[2*t+2];
    }
    if(value == 1){
        leaf = true;
        keys = new int[2*t];
        ptr = new BtreeNode*[2*t+1];
    }
}

int BtreeNode::getsize(){
    return size;
}
void BtreeNode::setSize(int value){
    size = value;
}
void BtreeNode::size_increase(){
    size++;
}
int* BtreeNode::getkeys(){
    return keys;
}
void BtreeNode::empty(){
    size = 0;
}
void BtreeNode::setkeys(int index , int value){
    keys[index] = value;
}
void BtreeNode::setptr(int index , BtreeNode* value){
    ptr[index] = value;
}

BtreeNode** BtreeNode::getptr(){
    return ptr;
}
int BtreeNode::gett(){
    return t;
}
int BtreeNode::is_leaf(){
    return leaf;
}
void BtreeNode::setleaf(bool value){
    leaf = value;
}

bool BtreeNode::insert_value(int value){
    if(leaf == true && size < 2*t){
        keys[size] = value;
        int i = size;
        while(i >= 1 && keys[i] < keys[i-1]){
            swap(keys[i],keys[i-1]);
            i--;
        }
        size++;
        return true;
    }
    return false;
}

BtreeNode::~BtreeNode(){
    delete[] keys;
    delete[] ptr;
}

class Btree{
    public:
        Btree(int,int);
        virtual ~Btree();

        BtreeNode* getroot();
        void insert(int);
        void insertInternal(int,BtreeNode*,BtreeNode*);
        BtreeNode* findParent(BtreeNode*,BtreeNode*);
        void show();
    private:
        BtreeNode* root;
        int t;
        int d;
        int data;
        int index;
};

Btree::Btree(int d_value,int t_value){
    root = nullptr;
    t = t_value;
    d = d_value;
    data = 0;
    index=0;
}

void Btree::insert(int value){
    if(root == nullptr){
        root = new BtreeNode(d,1);
        data++;
        root->setkeys(0,value);
        root->size_increase();
    }

    else{
        BtreeNode* curr = root;
        BtreeNode* prev = nullptr;

        while(curr->is_leaf() == false){
            prev = curr;
            for(int i=0;i<curr->getsize();i++){
                if(value < curr->getkeys()[i]){
                    curr = curr->getptr()[i];
                    break;
                }
                if(i == (curr->getsize()-1)){
                    curr = curr->getptr()[i+1];
                    break;
                }
            }
        }
        if(curr->getsize() < 2*d){
            curr->insert_value(value);
            curr->setptr(curr->getsize(),curr->getptr()[curr->getsize()-1]);
            curr->setptr(curr->getsize()-1,nullptr);
        }
        else{
            BtreeNode* node = new BtreeNode(d,1);
            data++;

            int tmp[2*d+1];
            for(int i=0;i<2*d;i++){
                tmp[i] = curr->getkeys()[i];
            }
            curr->empty();

            tmp[2*d] = value;
            int i = 2*d;
            while(i >= 1 && tmp[i] < tmp[i-1]){
                swap(tmp[i],tmp[i-1]);
                i--;
            }

            //curr ... node
            for(int i=0;i<d;i++){
                curr->insert_value(tmp[i]);
            }
            for(int i=d;i<=2*d;i++){
                node->insert_value(tmp[i]);
            }
            curr->setptr(curr->getsize(),node);
            node->setptr(node->getsize(),curr->getptr()[2*d]);
            curr->setptr(2*d,nullptr);

            if(curr == root){
                BtreeNode* newRoot = new BtreeNode(t,0);
                index++;
                
                newRoot->setkeys(0,node->getkeys()[0]);
                newRoot->size_increase();
                newRoot->setptr(0,curr);
                newRoot->setptr(1,node);
                root = newRoot;
            }
            else{
                insertInternal(node->getkeys()[0],prev,node);
            }
        }
    }
}

void Btree::insertInternal(int x,BtreeNode* curr,BtreeNode* child){
    if(curr->getsize() < 2*t+1){
        int i=0;
        while(x > curr->getkeys()[i] && i < curr->getsize()){
            i++;
        }
        for(int j = curr->getsize();j>i;j--){
            curr->setkeys(j,curr->getkeys()[j-1]);
        }
        for(int j = curr->getsize()+1;j>i+1;j--){
            curr->setptr(j,curr->getptr()[j-1]);
        }
        curr->setkeys(i,x);
        curr->size_increase();
        curr->setptr(i+1,child);
    }
    else{

        BtreeNode* newInternal = new BtreeNode(t,0);
        index++;
        int tmp[2*t+2];
        BtreeNode* tmpptr[2*t+3];

        for(int i=0;i<curr->getsize();i++){
            tmp[i] = curr->getkeys()[i];
        }
        for(int i=0;i<curr->getsize()+1;i++){
            tmpptr[i] = curr->getptr()[i];
        }

        int i=0;
        while(x > tmp[i] && i < (2*t+1)){
            i++;
        }
        for(int j = (2*t+1);j>i;j--){
            tmp[j] = tmp[j-1];
        }
        tmp[i] = x;

        for(int j = 2*t+2;j>i+2;j--){
            tmpptr[j] = tmpptr[j-1];
        }
        tmpptr[i+1] = child;

        curr->setSize(t);
        newInternal->setSize(t+1);
        for(int i=0,j=curr->getsize()+1;i<newInternal->getsize();i++,j++){
            newInternal->setkeys(i,tmp[j]);
        }
        for(int i=0,j=curr->getsize()+1;i<newInternal->getsize()+1;i++,j++){
            newInternal->setptr(i,tmpptr[j]);
        }
        for(int i=0;i<curr->getsize();i++){
            curr->setkeys(i,tmp[i]);
        }
        for(int i=0;i<curr->getsize()+1;i++){
            curr->setptr(i,tmpptr[i]);
        }

        if(curr == root){
            BtreeNode* newRoot = new BtreeNode(t,0);
            index++;
            newRoot->setkeys(0,tmp[curr->getsize()]);
            newRoot->setptr(0,curr);
            newRoot->setptr(1,newInternal);
            newRoot->size_increase();
            root = newRoot;
        }
        else{
            insertInternal(tmp[curr->getsize()],
                            findParent(root,curr),
                            newInternal);
        }
    }
}

BtreeNode* Btree::findParent(BtreeNode* curr,BtreeNode* child){
    BtreeNode* parent;
    if(curr->is_leaf() || ((curr->getptr()[0])->is_leaf())){
        return nullptr;
    }

    for(int i=0;i<curr->getsize()+1;i++){
        if(curr->getptr()[i] == child){
            parent = child;
            return parent;
        }
        else{
            parent = findParent(curr->getptr()[i],child);
            if(parent != nullptr){
                return parent;
            }
        }
    }
    return parent;
}

void Btree::show(){
    cout<<index<<" "<<data<<"   ";
    for(int i=0;i<root->getsize();i++){
        cout<<root->getkeys()[i]<<" ";
    }
    cout<<endl;
}

BtreeNode* Btree::getroot(){
    return root;
}

Btree::~Btree(){
    //dtor
}

int main(){
    int d,t;
    cin>>d>>t;

    Btree b(2,1);
    int x,y;
    cin>>x;
    while(x != 3){
        if(x == 1){
            cin>>y;
            b.insert(y);
        }
        if(x == 2){
            b.show();
        }
        cin>>x;
    }

}