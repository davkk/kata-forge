#include "kata_test.h"

int main() {
    { TEST("same");
        Node* _a = new Node{20, new Node{10, new Node{5, nullptr, new Node{7, nullptr, nullptr}}, new Node{15, nullptr, nullptr}}, new Node{50, new Node{30, new Node{29, nullptr, nullptr}, new Node{45, nullptr, nullptr}}, new Node{100, nullptr, nullptr}}};
        Node* _b = new Node{20, new Node{10, new Node{5, nullptr, new Node{7, nullptr, nullptr}}, new Node{15, nullptr, nullptr}}, new Node{50, new Node{30, new Node{29, nullptr, nullptr}, new Node{45, nullptr, nullptr}}, new Node{100, nullptr, nullptr}}};
        auto _r = compare(_a, _b);
        CHECK(_r == true);
    }

    { TEST("diff");
        Node* _a = new Node{20, new Node{10, new Node{5, nullptr, new Node{7, nullptr, nullptr}}, new Node{15, nullptr, nullptr}}, new Node{50, new Node{30, new Node{29, nullptr, nullptr}, new Node{45, nullptr, nullptr}}, new Node{100, nullptr, nullptr}}};
        Node* _b = new Node{20, new Node{10, new Node{5, nullptr, new Node{7, nullptr, nullptr}}, new Node{15, nullptr, nullptr}}, new Node{50, new Node{30, new Node{29, new Node{21, nullptr, nullptr}, nullptr}, new Node{45, nullptr, new Node{49, nullptr, nullptr}}}, nullptr}};
        auto _r = compare(_a, _b);
        CHECK(_r == false);
    }

    return 0;
}
