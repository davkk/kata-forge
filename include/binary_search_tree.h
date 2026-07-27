#pragma once
#include <bits/stdc++.h>
using namespace std;

struct BinarySearchTree {
    struct Node { int val; Node *left, *right; };
    Node* root;
    void insert(int val);
    void erase(int val);
    optional<int> find(int val);
    int length();
};
