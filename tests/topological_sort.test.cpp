#include <cassert>

int main() {
    { // basic
        int _n = 4;
        vector<pair<int64_t,int64_t>> _edges = {{0,1}, {1,2}, {3,0}};
        auto _r = solve(_n, _edges);
        { auto _exp = vector<int>{3, 0, 1, 2}; assert(_r == _exp); }
    }

    { // cycle
        int _n = 2;
        vector<pair<int64_t,int64_t>> _edges = {{0,1}, {1,0}};
        auto _r = solve(_n, _edges);
        { auto _exp = vector<int>{}; assert(_r == _exp); }
    }

    { // single
        int _n = 1;
        vector<pair<int64_t,int64_t>> _edges = {};
        auto _r = solve(_n, _edges);
        { auto _exp = vector<int>{0}; assert(_r == _exp); }
    }

    return 0;
}
