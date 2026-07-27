#include "kata_test.h"
#include <optional>

int main() { TEST("prefix_sum_after_point_update");
    auto _inst0 = FenwickTree{{5}};
    CHECK(_inst0.length() == 5);
    _inst0.add(1, 1);
    _inst0.add(2, 2);
    _inst0.add(3, 3);
    CHECK(_inst0.sum(1) == 1);
    CHECK(_inst0.sum(2) == 3);
    CHECK(_inst0.sum(3) == 6);
    CHECK(_inst0.sum(5) == 6);
    _inst0.add(3, 5);
    CHECK(_inst0.sum(3) == 11);
    CHECK(_inst0.sum(5) == 11);

    return 0;
}
