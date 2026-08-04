#include "kata_test.h"
#include <optional>

int main() { TEST("streaming_medians");
    MedianFinder _inst0;
    _inst0.insert(5);
    EQL(5.0, _inst0.getMedian());
    EQL(1, _inst0.size());
    _inst0.insert(10);
    EQL(7.5, _inst0.getMedian());
    EQL(2, _inst0.size());
    _inst0.insert(3);
    EQL(5.0, _inst0.getMedian());
    EQL(3, _inst0.size());
    _inst0.insert(8);
    EQL(6.5, _inst0.getMedian());
    EQL(4, _inst0.size());
    _inst0.insert(1);
    EQL(5.0, _inst0.getMedian());
    EQL(5, _inst0.size());

    return 0;
}
