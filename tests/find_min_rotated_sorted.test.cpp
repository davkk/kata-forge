#include "kata_test.h"

int main() {
    { TEST("rotation_near_middle");
        vector<int> _arr = {3, 4, 5, 1, 2};
        auto _r = find_min_rotated(_arr);
        EQL(1, _r);
    }

    { TEST("rotation_near_end");
        vector<int> _arr = {4, 5, 6, 7, 0, 1, 2};
        auto _r = find_min_rotated(_arr);
        EQL(0, _r);
    }

    { TEST("not_rotated");
        vector<int> _arr = {11, 13, 15, 17};
        auto _r = find_min_rotated(_arr);
        EQL(11, _r);
    }

    { TEST("single_element");
        vector<int> _arr = {1};
        auto _r = find_min_rotated(_arr);
        EQL(1, _r);
    }

    { TEST("two_elements");
        vector<int> _arr = {2, 1};
        auto _r = find_min_rotated(_arr);
        EQL(1, _r);
    }

    return 0;
}
