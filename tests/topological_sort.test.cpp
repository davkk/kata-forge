#include "kata_test.h"

int main() {
    { TEST("acyclic_graph");
        int _n = 4;
        vector<pair<int,int>> _edges = {{0,1}, {1,2}, {3,0}};
        auto _r = topological_sort(_n, _edges);
        { auto _exp = vector<int>{3, 0, 1, 2}; EQL(_exp, _r); }
    }

    { TEST("graph_with_cycle");
        int _n = 2;
        vector<pair<int,int>> _edges = {{0,1}, {1,0}};
        auto _r = topological_sort(_n, _edges);
        { auto _exp = vector<int>{}; EQL(_exp, _r); }
    }

    { TEST("single_node");
        int _n = 1;
        vector<pair<int,int>> _edges = {};
        auto _r = topological_sort(_n, _edges);
        { auto _exp = vector<int>{0}; EQL(_exp, _r); }
    }

    return 0;
}
