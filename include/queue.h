#pragma once
#include <bits/stdc++.h>
using namespace std;

struct Queue {
    struct Node { int val; Node* next; };
    Node *head, *tail;
    void enqueue(int x);
    optional<int> deque();
    optional<int> peek();
    int length();
};
