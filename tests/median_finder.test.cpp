#include "kata_test.h"
#include <optional>

int main() { TEST("streaming_medians");
    MedianFinder _inst0;
    _inst0.insert(5);
    CHECK(_inst0.getMedian() == 5.0);
    CHECK(_inst0.length() == 1);
    _inst0.insert(10);
    CHECK(_inst0.getMedian() == 7.5);
    CHECK(_inst0.length() == 2);
    _inst0.insert(3);
    CHECK(_inst0.getMedian() == 5.0);
    CHECK(_inst0.length() == 3);
    _inst0.insert(8);
    CHECK(_inst0.getMedian() == 6.5);
    CHECK(_inst0.length() == 4);
    _inst0.insert(1);
    CHECK(_inst0.getMedian() == 5.0);
    CHECK(_inst0.length() == 5);

    return 0;
}
