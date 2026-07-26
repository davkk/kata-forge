#include "kata_test.h"
#include <optional>

int main() { TEST("basic");
    HashSet _inst0;
    CHECK(_inst0.size() == 0);
    CHECK(_inst0.has(1) == false);
    _inst0.add(1);
    _inst0.add(2);
    CHECK(_inst0.has(1) == true);
    CHECK(_inst0.has(3) == false);
    CHECK(_inst0.size() == 2);
    _inst0.add(2);
    CHECK(_inst0.size() == 2);
    _inst0.remove(2);
    CHECK(_inst0.has(2) == false);
    CHECK(_inst0.size() == 1);
    _inst0.remove(1);
    CHECK(_inst0.has(1) == false);
    CHECK(_inst0.size() == 0);
    _inst0.add(100);
    _inst0.add(200);
    _inst0.add(300);
    CHECK(_inst0.size() == 3);

    return 0;
}
