#pragma once
#include <bits/stdc++.h>
using namespace std;

struct MinHeap {
    vector<int> data;
    void insert(int x);
    optional<int> deleteMin();
    int length();
};
