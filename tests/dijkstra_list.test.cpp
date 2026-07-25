#include <cassert>

int main() {
    { // path
        vector<vector<Edge>> _g = {{{1, 3}, {2, 1}}, {{0, 3}, {2, 4}, {4, 1}}, {{1, 4}, {3, 7}, {0, 1}}, {{2, 7}, {4, 5}, {6, 1}}, {{1, 1}, {3, 5}, {5, 2}}, {{6, 1}, {4, 2}, {2, 18}}, {{3, 1}, {5, 1}}};
        int _src = 0;
        int _sink = 6;
        auto _r = dfs(_g, _src, _sink);
        { auto _exp = vector<int>{0, 1, 4, 5, 6}; assert(_r.has_value() && *_r == _exp); }
    }

    return 0;
}
