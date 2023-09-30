#include "babble"

namespace qux {

class Bar {
public:
    float data[3];

    void set_data(float d[3]);
};

}


// test custom binding functions
BBL_MODULE(test0024) {
    bbl::Class<qux::Bar>()
    .value_type()
    .f(&qux::Bar::data)
    .m(&qux::Bar::set_data)
    ;

}