#include "kata_test.h"
#include <optional>

int main() { TEST("unite_and_query_connectivity");
    auto _inst0 = UnionFind{10};
    EQL(10, _inst0.size());
    CHECK(!_inst0.connected(0, 1));
    _inst0.unite(0, 1);
    CHECK(_inst0.connected(0, 1));
    _inst0.unite(1, 2);
    _inst0.unite(3, 4);
    _inst0.unite(5, 6);
    CHECK(_inst0.connected(0, 2));
    CHECK(!_inst0.connected(0, 4));
    _inst0.unite(2, 4);
    CHECK(_inst0.connected(0, 4));
    CHECK(!_inst0.connected(3, 5));

    // find_same_component
    { TEST("multi_step_transitive_connectivity");
        auto _inst1 = UnionFind{10};
        _inst1.unite(1, 2);
        _inst1.unite(3, 4);
        _inst1.unite(5, 6);
        _inst1.unite(7, 8);
        _inst1.unite(2, 3);
        CHECK(_inst1.connected(1, 4));
        CHECK(!_inst1.connected(1, 7));
    }

    return 0;
}
