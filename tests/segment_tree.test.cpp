#include "kata_test.h"
#include <optional>

int main() { TEST("basic");
    auto _inst0 = SegmentTree{{1, 3, 5, 7, 9, 11}};
    CHECK(_inst0.size() == 6);
    CHECK(_inst0.query(0, 2) == 9);
    CHECK(_inst0.query(3, 5) == 27);
    CHECK(_inst0.query(0, 5) == 36);
    _inst0.update(2, 10);
    CHECK(_inst0.query(0, 2) == 14);
    CHECK(_inst0.query(2, 2) == 10);

    return 0;
}
