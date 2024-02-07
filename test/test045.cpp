#include <babble>
#include <string>

namespace foo {

class Foo {
public:
    int foo1() const;
    int foo1();
    float foo1(float f);
};

}

#define BIND(TY) \
    bbl::Class<TY>() \
        .m((int (TY::*)() const) \
            &TY::foo1 \
        ) \
        .m((int (TY::*)()) \
            &TY::foo1 \
        ) \
        .m((float (TY::*)(float)) \
            &TY::foo1 \
        ) \


// test using macros to do entire class bindings
BBL_MODULE(test045) {
    BIND(foo::Foo);
}
