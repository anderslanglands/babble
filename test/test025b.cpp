#include "babble"

namespace qux {

class Foo {
public:
    void foo();
};

}

// test splitting modules across multiple files
BBL_MODULE(test0025) {
    bbl::Class<qux::Foo>()
    .value_type()
    .m(&qux::Foo::foo)
    ;

}

