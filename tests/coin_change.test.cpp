#include "kata_test.h"

int main() {
    { TEST("exact_coin_match");
        int _amount = 5;
        vector<int> _coins = {1, 2, 5};
        auto _r = coin_change(_amount, _coins);
        CHECK(_r == 1);
    }

    { TEST("impossible_no_combination");
        int _amount = 3;
        vector<int> _coins = {2};
        auto _r = coin_change(_amount, _coins);
        CHECK(_r == -1);
    }

    { TEST("zero_amount");
        int _amount = 0;
        vector<int> _coins = {1, 2, 5};
        auto _r = coin_change(_amount, _coins);
        CHECK(_r == 0);
    }

    { TEST("multi_coin_optimal");
        int _amount = 11;
        vector<int> _coins = {1, 2, 5};
        auto _r = coin_change(_amount, _coins);
        CHECK(_r == 3);
    }

    { TEST("greedy_optimal");
        int _amount = 100;
        vector<int> _coins = {1, 10, 25};
        auto _r = coin_change(_amount, _coins);
        CHECK(_r == 4);
    }

    return 0;
}
