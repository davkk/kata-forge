#include "kata_test.h"

int main() {
    { TEST("standard_mixed");
        vector<int> _arr = {-2, 1, -3, 4, -1, 2, 1, -5, 4};
        auto _r = kadane(_arr);
        CHECK(_r == 6);
    }

    { TEST("single_positive");
        vector<int> _arr = {1};
        auto _r = kadane(_arr);
        CHECK(_r == 1);
    }

    { TEST("all_positive_with_dip");
        vector<int> _arr = {5, 4, -1, 7, 8};
        auto _r = kadane(_arr);
        CHECK(_r == 23);
    }

    { TEST("single_negative");
        vector<int> _arr = {-1};
        auto _r = kadane(_arr);
        CHECK(_r == -1);
    }

    { TEST("all_negative");
        vector<int> _arr = {-2, -1};
        auto _r = kadane(_arr);
        CHECK(_r == -1);
    }

    return 0;
}
