#include "kata_test.h"

int main() {
    { TEST("rotated_at_mid");
        vector<int> _arr = {3, 4, 5, 1, 2};
        auto _r = find_min_rotated(_arr);
        CHECK(_r == 1);
    }

    { TEST("rotated_at_end");
        vector<int> _arr = {4, 5, 6, 7, 0, 1, 2};
        auto _r = find_min_rotated(_arr);
        CHECK(_r == 0);
    }

    { TEST("not_rotated");
        vector<int> _arr = {11, 13, 15, 17};
        auto _r = find_min_rotated(_arr);
        CHECK(_r == 11);
    }

    { TEST("single_element");
        vector<int> _arr = {1};
        auto _r = find_min_rotated(_arr);
        CHECK(_r == 1);
    }

    { TEST("two_elements_rotated");
        vector<int> _arr = {2, 1};
        auto _r = find_min_rotated(_arr);
        CHECK(_r == 1);
    }

    return 0;
}
