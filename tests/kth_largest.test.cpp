#include "kata_test.h"

int main() {
    { TEST("second_largest_unsorted");
        vector<int> _arr = {3, 2, 1, 5, 6, 4};
        int _k = 2;
        auto _r = kth_largest(_arr, _k);
        CHECK(_r == 5);
    }

    { TEST("fourth_largest_with_duplicates");
        vector<int> _arr = {3, 2, 3, 1, 2, 4, 5, 5, 6};
        int _k = 4;
        auto _r = kth_largest(_arr, _k);
        CHECK(_r == 4);
    }

    { TEST("single_element");
        vector<int> _arr = {1};
        int _k = 1;
        auto _r = kth_largest(_arr, _k);
        CHECK(_r == 1);
    }

    { TEST("two_elements");
        vector<int> _arr = {2, 1};
        int _k = 1;
        auto _r = kth_largest(_arr, _k);
        CHECK(_r == 2);
    }

    return 0;
}
