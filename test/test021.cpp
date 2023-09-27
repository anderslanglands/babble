#include "babble"
#include <string>

template <typename T>
class Bar {
    T t;
};

class Foo {
public:
    template <typename T>
    T get() const;
};

// test template function binding
BBL_MODULE(test0021) {
    bbl::Class<Foo>()
    .m((float*(Foo::*)())
        &Foo::get<float>, "get_float"
    )
    .m((float*(Foo::*)())
        &Foo::get<int>, "get_int"
    )
    .m((Bar<short>*(Foo::*)())
        &Foo::get<Bar<short>>, "get_bar_short"
    )
    ;

    bbl::Class<Bar<short>>("BarShort")
    ;
}