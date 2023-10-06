#include "babble"
#include <string>

template <typename T>
class Bar {
    T t;
};

#define EMPTY_SPACE \
 \
  \

class Foo {
public:
    template <typename T>
    T get() const;

    template <typename T>
    bool is() const;
};

// test template function binding
BBL_MODULE(test0021) {
    bbl::Class<Foo>()
    EMPTY_SPACE
    .m((float*(Foo::*)())
        &Foo::get<float>, "get_float"
    )
    .m((int*(Foo::*)())
        &Foo::get<int>, "get_int"
    )
    .m((bool (Foo::*)())
        &Foo::is<float>, "is_float"
    )
    .m((bool (Foo::*)())
        &Foo::is<int>, "is_int"
    )
    .m((Bar<short>*(Foo::*)())
        &Foo::get<Bar<short>>, "get_bar_short"
    )
    ;

    bbl::Class<Bar<short>>("BarShort")
    ;
}