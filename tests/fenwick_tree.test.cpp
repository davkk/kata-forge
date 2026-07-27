#include "kata_test.h"
#include <optional>

int main() { TEST("prefix_sum_after_point_update");
    auto _inst0 = FenwickTree{5};
    EQL(5, _inst0.length());
    _inst0.add(1, 1);
    _inst0.add(2, 2);
    _inst0.add(3, 3);
    EQL(1, _inst0.sum(1));
    EQL(3, _inst0.sum(2));
    EQL(6, _inst0.sum(3));
    EQL(6, _inst0.sum(5));
    _inst0.add(3, 5);
    EQL(11, _inst0.sum(3));
    EQL(11, _inst0.sum(5));

    return 0;
}
