#include "kata_test.h"

int main() {
    { TEST("present_in_middle");
        vector<int> _arr = {1, 3, 4, 69, 71, 81, 90, 99, 420, 1337, 69420};
        int _needle = 69;
        auto _r = search(_arr, _needle);
        CHECK(_r == true);
    }

    { TEST("absent_in_middle");
        vector<int> _arr = {1, 3, 4, 69, 71, 81, 90, 99, 420, 1337, 69420};
        int _needle = 1336;
        auto _r = search(_arr, _needle);
        CHECK(_r == false);
    }

    { TEST("present_at_end");
        vector<int> _arr = {1, 3, 4, 69, 71, 81, 90, 99, 420, 1337, 69420};
        int _needle = 69420;
        auto _r = search(_arr, _needle);
        CHECK(_r == true);
    }

    { TEST("absent_above_max");
        vector<int> _arr = {1, 3, 4, 69, 71, 81, 90, 99, 420, 1337, 69420};
        int _needle = 69421;
        auto _r = search(_arr, _needle);
        CHECK(_r == false);
    }

    { TEST("present_at_start");
        vector<int> _arr = {1, 3, 4, 69, 71, 81, 90, 99, 420, 1337, 69420};
        int _needle = 1;
        auto _r = search(_arr, _needle);
        CHECK(_r == true);
    }

    { TEST("absent_below_min");
        vector<int> _arr = {1, 3, 4, 69, 71, 81, 90, 99, 420, 1337, 69420};
        int _needle = 0;
        auto _r = search(_arr, _needle);
        CHECK(_r == false);
    }

    return 0;
}
