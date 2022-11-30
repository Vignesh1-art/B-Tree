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
            if(curr->get_key(index - 1) == key) {
                *index_node = curr;
            }
            curr = curr->get_child(index);
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
                int index = parent->insert_key(mid_key);
                parent->set_child(index,curr_node);
                parent->set_child(index + 1,other_node);
            }
            curr_node = parent;
        }
    }

    void delete_key(int key) {
        if(root == nullptr) {
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
                int borrow_key = right_sibling->get_key(0);
                right_sibling->remove_key(0);
                node->insert_key(borrow_key);
                int index = node->get_index_at_parent();
                BNode *parent = node->get_parent();
                parent->set_key(index,right_sibling->get_key(0));
            } else if(left_sibling && left_sibling->can_borrow()) {
                int last_index = left_sibling->get_size() - 1;
                int borrow_key = left_sibling->get_key(last_index);
                left_sibling->remove_key(last_index);
                last_index--;
                node->insert_key(borrow_key);
                int index = node->get_index_at_parent();
                BNode *parent = node->get_parent();
                parent->set_key(index - 1, left_sibling->get_key(last_index));
            } else {
                BNode *merge_node = curr_node->get_left_sibling();
                int parent_index = curr_node->get_index_at_parent() - 1;
                bool is_left = true;
                if(merge_node == nullptr) {
                    merge_node = curr_node->get_right_sibling();
                    parent_index++;
                    is_left = false;
                }
                BNode *parent = curr_node->get_parent();
                if(curr_node->is_leaf()) {
                    curr_node->merge_node(merge_node);
                    parent->remove_key(parent_index);
                    parent->set_child(parent_index,curr_node);
                } else {
                    int parent_key = parent->get_key(parent_index);
                    parent->remove_key(parent_index);
                    if(is_left) {
                        merge_node->insert_key(parent_key);
                        merge_node->merge_node(curr_node);
                        curr_node = merge_node;
                    } else {
                        curr_node->insert_key(parent_key);
                        curr_node->merge_node(merge_node);
                    }
                    parent->set_child(parent_index,curr_node);
                }
                if(parent->get_size() == 0 && parent == root) {
                    delete parent;
                    curr_node->clear_parent();
                    root = curr_node;
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
