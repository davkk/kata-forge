#include "kata_test.h"

int main() {
    { TEST("overlap_and_disjoint");
        vector<pair<int,int>> _pairs = {{1,3}, {2,6}, {8,10}, {15,18}};
        auto _r = merge_ranges(_pairs);
        CHECK((_r == vector<pair<int,int>>{{1,6}, {8,10}, {15,18}}));
    }

    { TEST("adjacent_touch");
        vector<pair<int,int>> _pairs = {{1,4}, {4,5}};
        auto _r = merge_ranges(_pairs);
        CHECK((_r == vector<pair<int,int>>{{1,5}}));
    }

    { TEST("disjoint");
        vector<pair<int,int>> _pairs = {{1,2}, {3,4}};
        auto _r = merge_ranges(_pairs);
        CHECK((_r == vector<pair<int,int>>{{1,2}, {3,4}}));
    }

    { TEST("empty_input");
        vector<pair<int,int>> _pairs = {};
        auto _r = merge_ranges(_pairs);
        CHECK((_r == vector<pair<int,int>>{}));
    }

    { TEST("fully_contained");
        vector<pair<int,int>> _pairs = {{1,10}, {2,3}, {4,5}};
        auto _r = merge_ranges(_pairs);
        CHECK((_r == vector<pair<int,int>>{{1,10}}));
    }

    return 0;
}
