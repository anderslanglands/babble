#include "babble"

class Foo {
public:
    int bar(float a);
    float x;
    float y;
    float z;

};

// Binding fields
BBL_MODULE(test014) {
    using namespace bbl;
    
    bbl::Class<Foo>("Foo")
        .m(&Foo::bar)
        .f(&Foo::x, "x")
        .f(&Foo::y, "y")
        .f(&Foo::z, "z")
        ;
}
