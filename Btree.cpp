#include "BNode.h"

class BTree {
    private:
    struct BNode *root;
    int n;
    public:
    BTree(int n) {
        this->n = n;
        root = nullptr;
    }

    BNode *get_root_node() {
        return root;
    }

    void print() {
        queue<BNode*> queue;
        queue.push(root);
        while(!queue.empty()) {
            int curr_size = queue.size();
            for(int i = 0; i < curr_size; i++) {
                BNode *curr = queue.front();
                curr->print_keys();
                queue.pop();
                for(int j = 0; j <= curr->get_size(); j++) {
                    BNode *t = curr->get_child(j);
                    if(t) {
                        queue.push(t);   
                    }
                }
            }
            cout<<endl;
        }
    }

    BNode *search_key(int key) {
        BNode *curr = root;
        while (!curr->is_leaf()) {
            int index = curr->search_greater_key(key);
            curr = curr->get_child(index);
        }
        return curr;
    }

    void insert_key(int key) {
        if(root == nullptr) {
            root = new BNode(n,true);
            root->insert_key(key);
            return;
        }
        BNode* node = search_key(key);
        node->insert_key(key);
        if(!node->is_full()) {
            return;
        }
        BNode *curr_node = node;
        while (curr_node && curr_node->is_full()) {
            int mid_key = curr_node->get_key(curr_node->get_size()/2);
            BNode *other_node = curr_node->split_node();
            BNode *parent = curr_node->get_parent();
            if(parent == nullptr) {
                root = new BNode(n,false);
                root->insert_key(mid_key);
                root->set_child(0,curr_node);
                root->set_child(1,other_node);
            } else {
                int index = parent->insert_key(mid_key);
                parent->set_child(index,curr_node);
                parent->set_child(index + 1,other_node);
            }
            curr_node = parent;
        }
    }

};
