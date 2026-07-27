#pragma once
#include <bits/stdc++.h>
using namespace std;

struct Trie {
    struct Node { Node* child[26]; bool isWord = false; };
    Node* root;
    void insert(const string& word);
    void remove(const string& word);
    vector<string> find(const string& prefix);
};
