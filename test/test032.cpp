// genbind test

#include <OpenColorIO/OpenColorIO.h>

#if 0
#include <functional>

namespace foo {

class Bar {
    
};

template <typename T>
class TBar {
    T bar;
public:
    TBar(T const&);
    T const& get() const;
};

typedef TBar<float> FloatBar;

struct Baz {
    int a;
    float b;
};

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

auto do_thing(Bar const& bar, Baz** baz) -> BarEnum;

using FooFn = std::function<int(IntBaz const& ib, FloatBar)>;
auto do_fn(FooFn& fn) -> void;

}
#endif