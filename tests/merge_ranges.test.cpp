#include "kata_test.h"

int main() {
    { TEST("overlap_and_disjoint");
        vector<pair<int64_t,int64_t>> _pairs = {{1,3}, {2,6}, {8,10}, {15,18}};
        auto _r = solve(_pairs);
        CHECK(_r == vector<pair<int64_t,int64_t>>{{1,6}, {8,10}, {15,18}});
    }

    { TEST("adjacent_touch");
        vector<pair<int64_t,int64_t>> _pairs = {{1,4}, {4,5}};
        auto _r = solve(_pairs);
        CHECK(_r == vector<pair<int64_t,int64_t>>{{1,5}});
    }

    { TEST("disjoint");
        vector<pair<int64_t,int64_t>> _pairs = {{1,2}, {3,4}};
        auto _r = solve(_pairs);
        CHECK(_r == vector<pair<int64_t,int64_t>>{{1,2}, {3,4}});
    }

    { TEST("empty_input");
        vector<pair<int64_t,int64_t>> _pairs = {};
        auto _r = solve(_pairs);
        CHECK(_r == vector<pair<int64_t,int64_t>>{});
    }

    { TEST("fully_contained");
        vector<pair<int64_t,int64_t>> _pairs = {{1,10}, {2,3}, {4,5}};
        auto _r = solve(_pairs);
        CHECK(_r == vector<pair<int64_t,int64_t>>{{1,10}});
    }

    return 0;
}
