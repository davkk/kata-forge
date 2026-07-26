#include "kata_test.h"

int main() {
    { TEST("1_coin_to_make_5");
        int64_t _amount = 5;
        vector<int> _coins = {1, 2, 5};
        auto _r = solve(_amount, _coins);
        CHECK(_r == 1);
    }

    { TEST("impossible_no_combination");
        int64_t _amount = 3;
        vector<int> _coins = {2};
        auto _r = solve(_amount, _coins);
        CHECK(_r == -1);
    }

    { TEST("zero_amount");
        int64_t _amount = 0;
        vector<int> _coins = {1, 2, 5};
        auto _r = solve(_amount, _coins);
        CHECK(_r == 0);
    }

    { TEST("optimal_3_coins_for_11");
        int64_t _amount = 11;
        vector<int> _coins = {1, 2, 5};
        auto _r = solve(_amount, _coins);
        CHECK(_r == 3);
    }

    { TEST("greedy_works_4_coins_for_100");
        int64_t _amount = 100;
        vector<int> _coins = {1, 10, 25};
        auto _r = solve(_amount, _coins);
        CHECK(_r == 4);
    }

    return 0;
}
