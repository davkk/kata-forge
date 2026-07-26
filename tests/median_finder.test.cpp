#include "kata_test.h"
#include <optional>

int main() { TEST("basic");
    MedianFinder _inst0;
    _inst0.insert(5);
    CHECK(_inst0.getMedian() == 5.0);
    CHECK(_inst0.size() == 1);
    _inst0.insert(10);
    CHECK(_inst0.getMedian() == 7.5);
    CHECK(_inst0.size() == 2);
    _inst0.insert(3);
    CHECK(_inst0.getMedian() == 5.0);
    CHECK(_inst0.size() == 3);
    _inst0.insert(8);
    CHECK(_inst0.getMedian() == 6.5);
    CHECK(_inst0.size() == 4);
    _inst0.insert(1);
    CHECK(_inst0.getMedian() == 5.0);
    CHECK(_inst0.size() == 5);

    return 0;
}
