#include "kata_test.h"

int main() {
    { TEST("find_69");
        vector<int> _arr = {1, 3, 4, 69, 71, 81, 90, 99, 420, 1337, 69420};
        int _needle = 69;
        auto _r = search(_arr, _needle);
        CHECK(_r == true);
    }

    { TEST("find_1336");
        vector<int> _arr = {1, 3, 4, 69, 71, 81, 90, 99, 420, 1337, 69420};
        int _needle = 1336;
        auto _r = search(_arr, _needle);
        CHECK(_r == false);
    }

    { TEST("find_69420");
        vector<int> _arr = {1, 3, 4, 69, 71, 81, 90, 99, 420, 1337, 69420};
        int _needle = 69420;
        auto _r = search(_arr, _needle);
        CHECK(_r == true);
    }

    { TEST("find_69421");
        vector<int> _arr = {1, 3, 4, 69, 71, 81, 90, 99, 420, 1337, 69420};
        int _needle = 69421;
        auto _r = search(_arr, _needle);
        CHECK(_r == false);
    }

    { TEST("find_1");
        vector<int> _arr = {1, 3, 4, 69, 71, 81, 90, 99, 420, 1337, 69420};
        int _needle = 1;
        auto _r = search(_arr, _needle);
        CHECK(_r == true);
    }

    { TEST("find_0");
        vector<int> _arr = {1, 3, 4, 69, 71, 81, 90, 99, 420, 1337, 69420};
        int _needle = 0;
        auto _r = search(_arr, _needle);
        CHECK(_r == false);
    }

    return 0;
}
