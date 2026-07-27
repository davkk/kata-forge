#include "kata_test.h"
#include <optional>

int main() { TEST("range_sum_after_point_update");
    auto _inst0 = SegmentTree({1, 3, 5, 7, 9, 11});
    EQL(6, _inst0.length());
    EQL(9, _inst0.query(0, 2));
    EQL(27, _inst0.query(3, 5));
    EQL(36, _inst0.query(0, 5));
    _inst0.update(2, 10);
    EQL(14, _inst0.query(0, 2));
    EQL(10, _inst0.query(2, 2));

    return 0;
}
