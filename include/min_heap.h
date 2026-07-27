#pragma once
#include <bits/stdc++.h>
using namespace std;

struct MinHeap {
    void push(int x);
    optional<int> pop();
    int size();
};
