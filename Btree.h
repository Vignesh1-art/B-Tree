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

    int get_min_key(BNode *child) {
        BNode *curr = child;
        while (!curr->is_leaf()) {
            curr = curr->get_child(0);
        }
        return curr->get_key(0);
    }

    BNode *get_root_node() {
        return root;
    }

    void print() {
        if(root == nullptr) {
            cout<<"Tree Empty"<<endl;
            return;
        }
        queue<BNode*> queue;
        queue.push(root);
        while(!queue.empty()) {
            int curr_size = queue.size();
            for(int i = 0; i < curr_size; i++) {
                BNode *curr = queue.front();
                curr->print_keys();
                queue.pop();
                if(curr->is_leaf()) {
                    continue;
                }
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
            if(curr->get_key(index) <= key) {
                curr = curr->get_child(index + 1);
            } else {
                curr = curr->get_child(index);
            }
        }
        return curr;
    }

    int find_max_key(BNode *node) {
        BNode *curr = node;
        while (!curr->is_leaf()) {
            curr = curr->get_child(curr->get_size());
        }
        return curr->get_key(curr->get_size() - 1);
    }

    void search_key(int key, BNode **node, BNode **index_node) {
        BNode *curr = root;
        *index_node = nullptr;
        *node = nullptr;
        while (!curr->is_leaf()) {
            int index = curr->search_greater_key(key);
            if(curr->get_key(index) == key) {
                *index_node = curr;
            }
            if(curr->get_key(index) <= key) {
                curr = curr->get_child(index + 1);
            } else {
                curr = curr->get_child(index);
            }
        }
        *node = curr;
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
                int index = curr_node->get_index_at_parent();
                curr_node->get_parent()->insert_key(index,mid_key);
                parent->set_child(index,curr_node);
                parent->set_child(index + 1,other_node);
            }
            curr_node = parent;
        }
    }

    void delete_key(int key) {
        if(root == nullptr) {
            cout<<"Tree Empty :: No keys Found"<<endl;
            return;
        }
        BNode *node, *index_node;
        search_key(key,&node,&index_node);
        if(node == nullptr) {
            cout<<"Key not found"<<endl;
            return;
        }

        {
            int index = node->search_key(key);
            if(index == -1) {
                return;
            }
            node->remove_key(index);
            if(node->get_size() == 0 && node == root) {
                delete node;
                root = nullptr;
                return;
            }
        }

        BNode *curr_node = node;
        while (curr_node && !curr_node->has_minimum()) {

            BNode *right_sibling = curr_node->get_right_sibling();
            BNode *left_sibling = curr_node->get_left_sibling();

            if(right_sibling && right_sibling->can_borrow()) {
                int index = curr_node->get_index_at_parent();
                BNode *parent = right_sibling->get_parent();
                if(parent->get_key(0) == key) {
                    int borrow_key = right_sibling->get_key(0);
                    parent->set_key(index,borrow_key);
                    curr_node->insert_key(curr_node->get_size(),borrow_key);
                } else {
                    int borrow_key = parent->get_key(index);
                    //Set 1th key in root key because 0th key will be removed
                    parent->set_key(index,right_sibling->get_key(1));
                    curr_node->insert_key(curr_node->get_size(),borrow_key);
                }
                curr_node->set_child(curr_node->get_size(), right_sibling->get_child(0));
                right_sibling->remove_key(0);

            } else if(left_sibling && left_sibling->can_borrow()) {
                int index = left_sibling->get_index_at_parent();
                BNode *parent = left_sibling->get_parent();
                if(parent->get_key(parent->get_size() - 1) == key) {
                    int borrow_key = left_sibling->get_key(left_sibling->get_size() - 1);
                    parent->set_key(index,borrow_key);
                    curr_node->insert_key(0,borrow_key);
                } else {
                    int borrow_key = parent->get_key(index);
                    //Set prev to last key as root key because last key will be removed
                    int t = left_sibling->get_key(left_sibling->get_size() - 2);
                    parent->set_key(index,t);
                    curr_node->insert_key(0,borrow_key);
                }
                curr_node->set_child(0,left_sibling->get_child(left_sibling->get_size()));
                left_sibling->remove_key(left_sibling->get_size() - 1);

            } else {
                BNode *merge_node = curr_node->get_left_sibling();
                int parent_index = curr_node->get_index_at_parent() - 1;
                BNode *parent = curr_node->get_parent();
                bool is_left = true;
                if(merge_node == nullptr) {
                    merge_node = curr_node->get_right_sibling();
                    parent_index = merge_node->get_index_at_parent() - 1;
                    is_left = false;
                }
                if(is_left) {
                    BNode *temp = curr_node;
                    curr_node = merge_node;
                    merge_node = temp;
                }
                if(curr_node->is_leaf()) {
                    curr_node->merge_node(merge_node);
                    parent->remove_key(parent_index);
                } else {
                    int parent_key = parent->get_key(parent_index);
                    curr_node->insert_key(curr_node->get_size(),parent_key);
                    curr_node->merge_node(merge_node);
                    parent->remove_key(parent_index);
                }
                parent->set_child(parent_index,curr_node);
                if(parent == root && parent->get_size() == 0) {
                    root = curr_node;
                    root->clear_parent();
                    delete parent;
                }
            }

            curr_node = curr_node->get_parent();
        }

        if(index_node) {
            int index = index_node->search_key(key);
            if(index == -1) {
                return;
            }
            int key = find_max_key(index_node->get_child(index + 1));
            index_node->set_key(index,key);
        }
    }

};
