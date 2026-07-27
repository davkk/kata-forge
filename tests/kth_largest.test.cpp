#include "kata_test.h"

int main() {
    { TEST("second_largest_unsorted");
        vector<int> _arr = {3, 2, 1, 5, 6, 4};
        int _k = 2;
        auto _r = kth_largest(_arr, _k);
        EQL(5, _r);
    }

    { TEST("fourth_largest_with_duplicates");
        vector<int> _arr = {3, 2, 3, 1, 2, 4, 5, 5, 6};
        int _k = 4;
        auto _r = kth_largest(_arr, _k);
        EQL(4, _r);
    }

    { TEST("single_element");
        vector<int> _arr = {1};
        int _k = 1;
        auto _r = kth_largest(_arr, _k);
        EQL(1, _r);
    }

    { TEST("two_elements");
        vector<int> _arr = {2, 1};
        int _k = 1;
        auto _r = kth_largest(_arr, _k);
        EQL(2, _r);
    }

    return 0;
}
