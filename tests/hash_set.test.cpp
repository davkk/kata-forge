#include <cassert>
#include <optional>

int main() {
    // basic
    HashSet _inst0;
    assert(_inst0.size() == 0);
    assert(_inst0.has(1) == false);
    _inst0.add(1);
    _inst0.add(2);
    assert(_inst0.has(1) == true);
    assert(_inst0.has(3) == false);
    assert(_inst0.size() == 2);
    _inst0.add(2);
    assert(_inst0.size() == 2);
    _inst0.remove(2);
    assert(_inst0.has(2) == false);
    assert(_inst0.size() == 1);
    _inst0.remove(1);
    assert(_inst0.has(1) == false);
    assert(_inst0.size() == 0);
    _inst0.add(100);
    _inst0.add(200);
    _inst0.add(300);
    assert(_inst0.size() == 3);

    return 0;
}
