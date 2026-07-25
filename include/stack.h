#pragma once
#include <bits/stdc++.h>
using namespace std;

struct Stack {
    void push(int x);
    optional<int> pop();
    optional<int> peek();
    int size();
};
