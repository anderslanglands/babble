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
    Foo(T const&);

    template <typename T>
    T const& get() const;

    template <typename T>
    bool is() const;
};

#define METHODS(TY, NAME) \
    .ctor(bbl::Class<Foo>::Ctor<TY const&>("value"), "from_" NAME) \
    .m((TY const& (Foo::*)())                        \
        &Foo::get<TY>, "get_" NAME)                  \
    .m((bool (Foo::*)())                             \
        &Foo::is<TY>, "is_" NAME)                    



// test template function binding
BBL_MODULE(test0021) {
    bbl::Class<Foo>()
    EMPTY_SPACE
    METHODS(int, "int")
    METHODS(float, "float")
    ;

    bbl::Class<Bar<short>>("BarShort")
    ;
}
