#include "kata_test.h"

int main() {
    { TEST("found_45");
        Node* _tree = new Node{20, new Node{10, new Node{5, nullptr, new Node{7, nullptr, nullptr}}, new Node{15, nullptr, nullptr}}, new Node{50, new Node{30, new Node{29, nullptr, nullptr}, new Node{45, nullptr, nullptr}}, new Node{100, nullptr, nullptr}}};
        int _needle = 45;
        auto _r = bfs(_tree, _needle);
        CHECK(_r == true);
    }

    { TEST("found_7");
        Node* _tree = new Node{20, new Node{10, new Node{5, nullptr, new Node{7, nullptr, nullptr}}, new Node{15, nullptr, nullptr}}, new Node{50, new Node{30, new Node{29, nullptr, nullptr}, new Node{45, nullptr, nullptr}}, new Node{100, nullptr, nullptr}}};
        int _needle = 7;
        auto _r = bfs(_tree, _needle);
        CHECK(_r == true);
    }

    { TEST("missing");
        Node* _tree = new Node{20, new Node{10, new Node{5, nullptr, new Node{7, nullptr, nullptr}}, new Node{15, nullptr, nullptr}}, new Node{50, new Node{30, new Node{29, nullptr, nullptr}, new Node{45, nullptr, nullptr}}, new Node{100, nullptr, nullptr}}};
        int _needle = 69;
        auto _r = bfs(_tree, _needle);
        CHECK(_r == false);
    }

    return 0;
}
