#include "babble"

namespace qux {

class Bar {
public:
    void bar();
};

}

// test splitting modules across multiple files
BBL_MODULE(test0025) {
    bbl::Class<qux::Bar>()
    .value_type()
    .m(&qux::Bar::bar)
    ;

}
