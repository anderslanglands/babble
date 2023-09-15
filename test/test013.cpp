#include "babble"

class Foo {
public:
    int bar(float a);
};

// Test that binding a method twice is a warning
BBL_MODULE(test013) {
    using namespace bbl;
    
    bbl::Class<Foo>("Foo")
        .m(&Foo::bar)
        .m(&Foo::bar)
        ;
}
