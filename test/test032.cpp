// genbind test

#if 1
#include <OpenColorIO/OpenColorIO.h>

#else
#include <functional>

namespace foo {

class Bar {
public:
    void do_thing(int a);
    void do_thing(float a);

    template <typename T>
    void template_thing(T const& thing);

    class BarInner {
    public:
        int foo();

        enum InnerEnum {
            ONE=1,
            TWO=2
        };
    };
};

template <typename T>
class TBar {
    T bar;
public:
    TBar(T const&);
    T const& get() const;
    template <typename U>
    void set(U u);
};

typedef TBar<float> FloatBar;

namespace inner {
struct Baz {
    int a;
    float b;
};
}

template <typename T>
struct TBaz {
    T baz;
};

using IntBaz = TBaz<int>;
typedef TBaz<float> FloatBaz;

enum BarEnum {
    BAR = 0,
    BAZ = 1,
    FOO = 2,
};

auto do_thing(Bar const& bar, inner::Baz** baz) -> BarEnum;

template <typename T, typename U>
T do_template_thing(U const& u);

using FooFn = std::function<int(IntBaz const& ib, FloatBar)>;
auto do_fn(FooFn& fn) -> void;

}
#endif