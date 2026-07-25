#include <cassert>

int main() {
    { // a
        vector<pair<int64_t,int64_t>> _pairs = {{1,3}, {2,6}, {8,10}, {15,18}};
        auto _r = solve(_pairs);
        assert(_r == vector<pair<int64_t,int64_t>>{{1,6}, {8,10}, {15,18}});
    }

    { // b
        vector<pair<int64_t,int64_t>> _pairs = {{1,4}, {4,5}};
        auto _r = solve(_pairs);
        assert(_r == vector<pair<int64_t,int64_t>>{{1,5}});
    }

    { // c
        vector<pair<int64_t,int64_t>> _pairs = {{1,2}, {3,4}};
        auto _r = solve(_pairs);
        assert(_r == vector<pair<int64_t,int64_t>>{{1,2}, {3,4}});
    }

    { // d
        vector<pair<int64_t,int64_t>> _pairs = {};
        auto _r = solve(_pairs);
        assert(_r == vector<pair<int64_t,int64_t>>{});
    }

    { // e
        vector<pair<int64_t,int64_t>> _pairs = {{1,10}, {2,3}, {4,5}};
        auto _r = solve(_pairs);
        assert(_r == vector<pair<int64_t,int64_t>>{{1,10}});
    }

    return 0;
}
