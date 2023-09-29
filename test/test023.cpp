#include "babble"

namespace qux {

class Foo {
    float _data[3];

public:
    float length();
    float normalize();
};

}

class Bar {
public:
    float x, y, z;
};


// test custom binding functions
BBL_MODULE(test0023) {
    bbl::Class<qux::Foo>()
    .m(&qux::Foo::length)
    .replace_with<Bar>()
    .m(&qux::Foo::normalize)
    ;

}