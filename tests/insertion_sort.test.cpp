#include "kata_test.h"

int main() {
    { TEST("basic");
        vector<int> _arr = {9, 3, 7, 4, 69, 420, 42};
        insertion_sort(_arr);
        { auto _exp = vector<int>{3, 4, 7, 9, 42, 69, 420}; CHECK(_arr == _exp); }
    }

    { TEST("already_sorted");
        vector<int> _arr = {1, 2, 3};
        insertion_sort(_arr);
        { auto _exp = vector<int>{1, 2, 3}; CHECK(_arr == _exp); }
    }

    { TEST("reverse");
        vector<int> _arr = {5, 4, 3, 2, 1};
        insertion_sort(_arr);
        { auto _exp = vector<int>{1, 2, 3, 4, 5}; CHECK(_arr == _exp); }
    }

    { TEST("single");
        vector<int> _arr = {42};
        insertion_sort(_arr);
        { auto _exp = vector<int>{42}; CHECK(_arr == _exp); }
    }

    { TEST("empty");
        vector<int> _arr = {};
        insertion_sort(_arr);
        { auto _exp = vector<int>{}; CHECK(_arr == _exp); }
    }

    { TEST("duplicates");
        vector<int> _arr = {3, 1, 2, 1, 3};
        insertion_sort(_arr);
        { auto _exp = vector<int>{1, 1, 2, 3, 3}; CHECK(_arr == _exp); }
    }

    return 0;
}
