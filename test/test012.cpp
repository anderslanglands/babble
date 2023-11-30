#include "babble"

#include <string>

namespace qux {

class Base {
public:
    virtual ~Base();
    virtual void bar() = 0;
};

class Foo : public Base {
public:
    virtual ~Foo();
    virtual void bar() override;
};

}

/// Test binding abstract base classes and virtual functions
BBL_MODULE(test012) {
    bbl::Class<qux::Base>()
        .m(&qux::Base::bar)
    ;

    bbl::Class<qux::Foo>()
        .ctor(bbl::Class<qux::Foo>::Ctor<>())
        .m(&qux::Foo::bar)
        ;
}
