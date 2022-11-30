#ifndef BNODE
#define BNODE

#include<vector>
#include<queue>
#include<iostream>
using namespace std;

class BNode {
    vector<int> keys;
    vector<BNode *> children;
    int size;
    bool _is_leaf;
    int n;
    BNode *parent;
    public:
    BNode(int n,bool is_leaf) {
        keys = vector<int>(n);
        children = vector<BNode *>(n + 1);
        size = 0;
        _is_leaf = is_leaf;
        this->n = n;
        parent = nullptr;
    }

    void clear_parent() {
        parent = nullptr;
    }

    bool is_full() {
        return size == n;
    }

    bool is_leaf() {
        return _is_leaf;
    }

    BNode* get_child(int index) {
        return children[index];
    }

    int get_key(int index) {
        return keys[index];
    }

    void set_key(int index, int key) {
        keys[index] = key;
    }

    int get_size() {
        return size;
    }

    void set_child(int index, BNode *child) {
        children[index] = child;
        if(child) {
            children[index]->parent = this;
        }
    }

    BNode *get_parent() {
        return parent;
    }

    int insert_key(int key) {
        if(size == n) {
            return -1;
        }
        int i;
        for(i = size; i > 0 && keys[i - 1] > key; i--) {
            keys[i] = keys[i - 1];
        }
        keys[i] = key;
        size++;
        return i;
    }

    void print_keys() {
        cout<<"| ";
        for(int i = 0; i < size; i++) {
            cout<<keys[i]<<" ";
        }
        cout<<"|";
    }

    int search_greater_key(int key) {
        for(int i = 0; i < size; i++) {
            if(keys[i] > key) {
                return i;
            }
        }
        return size;
    }

    int search_key(int key) {
        for(int i = 0; i < size; i++) {
            if(keys[i] == key) {
                return i;
            }
        }
        return -1;
    }

    BNode *get_left_sibling() {
        if(parent == nullptr) {
            return nullptr;
        }
        int index = -1;
        for(int i = 0; i < parent->size + 1; i++) {
            if(parent->children[i] == this) {
                index = i;
                break;
            }
        }
        if(index == 0) {
            return nullptr;
        }
        return parent->children[index - 1];
    }

    BNode *get_right_sibling() {
        if(parent == nullptr) {
            return nullptr;
        }
        int index = -1;
        for(int i = 0; i < parent->size + 1; i++) {
            if(parent->children[i] == this) {
                index = i;
                break;
            }
        }
        if(index == parent->size) {
            return nullptr;
        }
        return parent->children[index + 1];
    }

    bool can_borrow() {
        return size > (n/2);
    }

    bool has_minimum() {
        return size >= (n/2);
    }

    void remove_key(int index) {
        for(int i = index; i < size - 1; i++) {
            keys[i] = keys[i + 1];
            children[i] = children[i + 1];
        }
        children[size - 1] = children[size];
        size--;
    }

    BNode *split_node() {
        BNode *other_node = new BNode(n,_is_leaf);
        int mid_index = size/2;
        int offset = _is_leaf ? 0 : 1;
        for(int i = mid_index + offset; i < size; i++) {
            int index = i - mid_index - offset;
            other_node->keys[index] = keys[i];
            other_node->set_child(index,children[i]);
            other_node->size++;
        }
        other_node->set_child(other_node->size,children[size]);
        size = mid_index;
        return other_node;
    }

    void merge_node(BNode *node) {
        if(node == nullptr) {
            return;
        }
        int offset = size;
        for(int i = 0; i < node->size; i++) {
            keys[offset + i] = node->keys[i];
            set_child(offset + i,node->children[i]);
            size++;
        }
        set_child(size,node->children[node->size]);
        delete node;
    }

    int get_index_at_parent() {

        for (int i = 0; i <= parent->size; i++) {
            if(parent->children[i] == this) {
                return i;
            }
        }
        return -1;
    }

};

#endif