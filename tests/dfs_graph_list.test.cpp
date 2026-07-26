#include "kata_test.h"

int main() {
    { TEST("shortest_path_0_to_6_via_1_4_5");
        vector<vector<Edge>> _g = {{{1, 3}, {2, 1}}, {{4, 1}}, {{3, 7}}, {}, {{1, 1}, {3, 5}, {5, 2}}, {{2, 18}, {6, 1}}, {{3, 1}}};
        int _src = 0;
        int _sink = 6;
        auto _r = dfs(_g, _src, _sink);
        { auto _exp = vector<int>{0, 1, 4, 5, 6}; CHECK(_r.has_value() && *_r == _exp); }
    }

    { TEST("sink_6_to_source_0_no_path");
        vector<vector<Edge>> _g = {{{1, 3}, {2, 1}}, {{4, 1}}, {{3, 7}}, {}, {{1, 1}, {3, 5}, {5, 2}}, {{2, 18}, {6, 1}}, {{3, 1}}};
        int _src = 6;
        int _sink = 0;
        auto _r = dfs(_g, _src, _sink);
        CHECK(!_r.has_value());
    }

    return 0;
}
