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

BBL_MODULE(test043) {
    bbl::Class<foo::Foo>()
        .m((int (foo::Foo::*)() const)
            &foo::Foo::foo1
        )
        .m((int (foo::Foo::*)())
            &foo::Foo::foo1
        )
        .ignore((float (foo::Foo::*)(float))
            &foo::Foo::foo1
        )
    ;
}
