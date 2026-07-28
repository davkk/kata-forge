#pragma once
#include <bits/stdc++.h>
using namespace std;

struct Stack {
    vector<int> data;
    void push(int x);
    optional<int> pop();
    optional<int> peek();
    int size();
};
