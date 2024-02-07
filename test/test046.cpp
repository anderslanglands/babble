#include <babble>
#include <string>

namespace bblext {

void normal_thing() {}

}

namespace foo {

class Foo {
public:
};

}

// Test binding a lambda directly
BBL_MODULE(test046) {
    bbl::Class<foo::Foo>();

    bbl::fn([](foo::Foo const& _this, int a) -> float {
        return 17.0f;
    }, "Foo_do_thing");

    bbl::fn(&bblext::normal_thing);
}
