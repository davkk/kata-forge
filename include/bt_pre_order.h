#pragma once
#include <bits/stdc++.h>
using namespace std;

struct Node { int val; Node* left; Node* right; };

vector<int> pre_order(Node* root);
