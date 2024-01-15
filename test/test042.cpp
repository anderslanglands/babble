#include <babble>
#include <string>

namespace foo {

class Abstract {
public:
    virtual void do_pure_thing() = 0;
    void public_method();
};

class Base1 : public Abstract {

};

class Base2 : public Base1 {
    virtual void do_pure_thing();
};

}

BBL_MODULE(test042) {
    bbl::Class<foo::Abstract>()
        .m(&foo::Abstract::do_pure_thing)
        .m(&foo::Abstract::public_method)
    ;

    bbl::Class<foo::Base1>()
    ;

    bbl::Class<foo::Base2>()
    ;

}
