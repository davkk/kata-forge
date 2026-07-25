#include <cassert>

int main() {
    { // path
        auto _g = {{0, 3, 1, 0, 0, 0, 0}, {0, 0, 0, 0, 1, 0, 0}, {0, 0, 0, 7, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0}, {0, 1, 0, 5, 0, 2, 0}, {0, 0, 18, 0, 0, 0, 1}, {0, 0, 0, 1, 0, 0, 0}};
        int _src = 0;
        int _sink = 6;
        auto _r = solve(_g, _src, _sink);
        { auto _exp = vector<int>{0, 1, 4, 5, 6}; assert(_r.has_value() && *_r == _exp); }
    }

    { // none
        auto _g = {{0, 3, 1, 0, 0, 0, 0}, {0, 0, 0, 0, 1, 0, 0}, {0, 0, 0, 7, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0}, {0, 1, 0, 5, 0, 2, 0}, {0, 0, 18, 0, 0, 0, 1}, {0, 0, 0, 1, 0, 0, 0}};
        int _src = 6;
        int _sink = 0;
        auto _r = solve(_g, _src, _sink);
        assert(!_r.has_value());
    }

    return 0;
}
