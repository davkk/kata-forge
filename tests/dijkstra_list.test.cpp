#include "kata_test.h"

int main() {
    { TEST("shortest_path_0_to_6_via_1_4_5");
        vector<vector<Edge>> _g = {{{1, 3}, {2, 1}}, {{0, 3}, {2, 4}, {4, 1}}, {{1, 4}, {3, 7}, {0, 1}}, {{2, 7}, {4, 5}, {6, 1}}, {{1, 1}, {3, 5}, {5, 2}}, {{6, 1}, {4, 2}, {2, 18}}, {{3, 1}, {5, 1}}};
        int _src = 0;
        int _sink = 6;
        auto _r = dijkstra(_g, _src, _sink);
        { auto _exp = vector<int>{0, 1, 4, 5, 6}; CHECK(_r.has_value() && *_r == _exp); }
    }

    { TEST("sink_not_reachable_from_source");
        vector<vector<Edge>> _g = {{{1, 1}}, {{2, 1}}, {}, {}};
        int _src = 0;
        int _sink = 3;
        auto _r = dijkstra(_g, _src, _sink);
        CHECK(!_r.has_value());
    }

    return 0;
}
