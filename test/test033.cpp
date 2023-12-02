#include <babble>
#include <string_view>

namespace qux {

struct Foo {
    bool set_name(std::string_view name);
};

std::string_view get_name();

}

// test lambda wrapping
BBL_MODULE(test033) {
    bbl::Class<qux::Foo>()
        // we can override the the parameters we use to call a method by inserting
        // the body of a lambda that does all the conversions for us.
        // this is exactly the same as manually creating and binding a wrapper
        // function declared in bblext
        .m(bbl::Wrap(
            &qux::Foo::set_name,
            [](qux::Foo& foo, char const* name) -> int {
                return foo.set_name(name);
            }
        ), "set_name_wrapped")
    ;

    bbl::fn(bbl::Wrap(&qux::get_name, [](char const** chars, size_t* len) -> void {
        std::string_view sv = qux::get_name();
        *chars = sv.data();
        *len = sv.size();
    }), "get_name_wrapped");
}