#include "kata_test.h"

int main() {
    { TEST("acyclic_graph");
        int _n = 4;
        vector<pair<int,int>> _edges = {{0,1}, {1,2}, {3,0}};
        auto _r = topological_sort(_n, _edges);
        { auto _exp = vector<int>{3, 0, 1, 2}; CHECK(_r == _exp); }
    }

    { TEST("graph_with_cycle");
        int _n = 2;
        vector<pair<int,int>> _edges = {{0,1}, {1,0}};
        auto _r = topological_sort(_n, _edges);
        { auto _exp = vector<int>{}; CHECK(_r == _exp); }
    }

    { TEST("single_node");
        int _n = 1;
        vector<pair<int,int>> _edges = {};
        auto _r = topological_sort(_n, _edges);
        { auto _exp = vector<int>{0}; CHECK(_r == _exp); }
    }

    return 0;
}
