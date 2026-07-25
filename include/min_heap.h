#pragma once
#include <bits/stdc++.h>
using namespace std;

struct MinHeap {
    void push(int64_t x);
    optional<int64_t> pop();
    int size();
};
