#include "kata_test.h"

int main() {
    { TEST("reachable_sink_finds_path");
        vector<vector<Edge>> _g = {{{1, 3}, {2, 1}}, {{4, 1}}, {{3, 7}}, {}, {{1, 1}, {3, 5}, {5, 2}}, {{2, 18}, {6, 1}}, {{3, 1}}};
        int _src = 0;
        int _sink = 6;
        auto _r = bfs(_g, _src, _sink);
        VAL(_r, (vector<int>{0, 1, 4, 5, 6}));
    }

    { TEST("unreachable_sink_returns_empty");
        vector<vector<Edge>> _g = {{{1, 3}, {2, 1}}, {{4, 1}}, {{3, 7}}, {}, {{1, 1}, {3, 5}, {5, 2}}, {{2, 18}, {6, 1}}, {{3, 1}}};
        int _src = 6;
        int _sink = 0;
        auto _r = bfs(_g, _src, _sink);
        CHECK(!_r.has_value());
    }

    return 0;
}
