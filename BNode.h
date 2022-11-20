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
            if(keys[i] >= key) {
                return i;
            }
        }
        return size;
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
};

#endif