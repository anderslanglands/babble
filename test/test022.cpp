#include "babble"
#include <string>

template <typename T>
class Bar {
    T t;
};

class Foo {
public:
    template <typename T>
    T get() const {
        return 0;
    }
};

namespace bblext {

float custom_foo(Foo const& foo) {
    return foo.get<float>();
}

}

// test custom binding functions
BBL_MODULE(test0022) {
    bbl::Class<Foo>()
    ;

    bbl::fn(&bblext::custom_foo);
}