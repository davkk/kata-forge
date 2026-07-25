#include <cassert>

int main() {
    { // path
        vector<vector<Edge>> _g = {{{1, 3}, {2, 1}}, {{4, 1}}, {{3, 7}}, {}, {{1, 1}, {3, 5}, {5, 2}}, {{2, 18}, {6, 1}}, {{3, 1}}};
        int _src = 0;
        int _sink = 6;
        auto _r = dfs(_g, _src, _sink);
        { auto _exp = vector<int>{0, 1, 4, 5, 6}; assert(_r.has_value() && *_r == _exp); }
    }

    { // none
        vector<vector<Edge>> _g = {{{1, 3}, {2, 1}}, {{4, 1}}, {{3, 7}}, {}, {{1, 1}, {3, 5}, {5, 2}}, {{2, 18}, {6, 1}}, {{3, 1}}};
        int _src = 6;
        int _sink = 0;
        auto _r = dfs(_g, _src, _sink);
        assert(!_r.has_value());
    }

    return 0;
}
