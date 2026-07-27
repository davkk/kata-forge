#pragma once
#include <bits/stdc++.h>
using namespace std;

struct RingBuffer {
    int cap;
    void push(int x);
    optional<int> pop();
    optional<int> get(int i);
    int size();
};
