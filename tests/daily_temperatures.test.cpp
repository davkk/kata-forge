#include "kata_test.h"

int main() {
    { TEST("mixed_temps");
        vector<int> _arr = {73, 74, 75, 71, 69, 72, 76, 73};
        auto _r = daily_temperatures(_arr);
        { auto _exp = vector<int>{1, 1, 4, 2, 1, 1, 0, 0}; EQL(_exp, _r); }
    }

    { TEST("monotonic_increasing");
        vector<int> _arr = {30, 40, 50, 60};
        auto _r = daily_temperatures(_arr);
        { auto _exp = vector<int>{1, 1, 1, 0}; EQL(_exp, _r); }
    }

    { TEST("short_increasing");
        vector<int> _arr = {30, 60, 90};
        auto _r = daily_temperatures(_arr);
        { auto _exp = vector<int>{1, 1, 0}; EQL(_exp, _r); }
    }

    { TEST("monotonic_decreasing");
        vector<int> _arr = {90, 80, 70};
        auto _r = daily_temperatures(_arr);
        { auto _exp = vector<int>{0, 0, 0}; EQL(_exp, _r); }
    }

    return 0;
}
