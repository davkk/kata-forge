#pragma once
#include <bits/stdc++.h>
using namespace std;

struct Node { int val; Node* left; Node* right; };

bool dfs(Node* root, int needle);
