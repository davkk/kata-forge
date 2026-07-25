#pragma once
#include <bits/stdc++.h>
using namespace std;

struct Queue {
    void push(int x);
    optional<int> pop();
    optional<int> front();
    int size();
};
