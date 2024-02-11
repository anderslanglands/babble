#include <babble>
#include <string>

namespace bblext {

void normal_thing() {}

}

namespace foo {

class Foo {
public:
    float f = 42.0f;
};

}

// Test binding a lambda directly
BBL_MODULE(test046) {
    bbl::Class<foo::Foo>();

    bbl::fn([](foo::Foo const& _this, int a) -> float {
        return _this.f;
    }, "Foo_do_thing");

    bbl::fn((void (*)())&bblext::normal_thing);
}
