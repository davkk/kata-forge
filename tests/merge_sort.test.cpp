#include "kata_test.h"

int main() {
    { TEST("unsorted_mixed");
        vector<int> _arr = {9, 3, 7, 4, 69, 420, 42};
        merge_sort(_arr);
        { auto _exp = vector<int>{3, 4, 7, 9, 42, 69, 420}; EQL(_exp, _arr); }
    }

    { TEST("already_sorted");
        vector<int> _arr = {1, 2, 3};
        merge_sort(_arr);
        { auto _exp = vector<int>{1, 2, 3}; EQL(_exp, _arr); }
    }

    { TEST("reverse");
        vector<int> _arr = {5, 4, 3, 2, 1};
        merge_sort(_arr);
        { auto _exp = vector<int>{1, 2, 3, 4, 5}; EQL(_exp, _arr); }
    }

    { TEST("single_element");
        vector<int> _arr = {42};
        merge_sort(_arr);
        { auto _exp = vector<int>{42}; EQL(_exp, _arr); }
    }

    { TEST("empty");
        vector<int> _arr = {};
        merge_sort(_arr);
        { auto _exp = vector<int>{}; EQL(_exp, _arr); }
    }

    { TEST("duplicates");
        vector<int> _arr = {3, 1, 2, 1, 3};
        merge_sort(_arr);
        { auto _exp = vector<int>{1, 1, 2, 3, 3}; EQL(_exp, _arr); }
    }

    return 0;
}
