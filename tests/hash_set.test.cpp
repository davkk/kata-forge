#include "kata_test.h"
#include <optional>

int main() { TEST("add_dedup_remove_size");
    HashSet _inst0;
    EQL(0, _inst0.size());
    CHECK(!_inst0.contains(1));
    _inst0.add(1);
    _inst0.add(2);
    CHECK(_inst0.contains(1));
    CHECK(!_inst0.contains(3));
    EQL(2, _inst0.size());
    _inst0.add(2);
    EQL(2, _inst0.size());
    _inst0.remove(2);
    CHECK(!_inst0.contains(2));
    EQL(1, _inst0.size());
    _inst0.remove(1);
    CHECK(!_inst0.contains(1));
    EQL(0, _inst0.size());
    _inst0.add(100);
    _inst0.add(200);
    _inst0.add(300);
    EQL(3, _inst0.size());

    return 0;
}
