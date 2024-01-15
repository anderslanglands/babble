#include <babble>
#include <string>

namespace foo {

class Abstract {
protected:
    virtual void do_protected_thing();
    Abstract(){}

public:
    virtual void do_pure_thing() = 0;

    void public_method();
};

}

namespace bblext {

class Base : public foo::Abstract {
public:
    Base() {}

    virtual void do_protected_thing() {
        foo::Abstract::do_protected_thing();
    }

    void do_protected_thing_base() {
        foo::Abstract::do_protected_thing();
    }
};

}

BBL_MODULE(test041) {
    bbl::Class<foo::Abstract>()
        .m(&foo::Abstract::do_pure_thing)
        .m(&foo::Abstract::public_method)
    ;

    bbl::Superclass<bblext::Base>()
        .m(&bblext::Base::do_protected_thing)
        .m(&bblext::Base::do_protected_thing_base)
    ;
}
