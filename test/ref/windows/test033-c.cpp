
#include <string_view>

#if defined(__GNUC__) || defined(__clang__)
#  define BBL_ALIGN(x) __attribute__ ((aligned(x)))
#elif defined(_MSC_VER)
#  define BBL_ALIGN(x) __declspec(align(x))
#else
#  error "Unknown compiler; can't define ALIGN"
#endif

#include <stddef.h>
#include <exception>
#include <thread>
#include <string>

static thread_local std::string _bbl_error_message;

namespace bblext {

auto Foo_set_name_wrapped(qux::Foo& foo, char const* name) -> int {
                return foo.set_name(name);
            }
auto get_name_wrapped(char const** chars, size_t* len) -> void {
        std::string_view sv = qux::get_name();
        *chars = sv.data();
        *len = sv.size();
    }
}


using test033_Foo_t = qux::Foo;

extern "C" {
int test033_Foo_dtor(test033_Foo_t* _this) {
    delete _this;
    return 0;
}

int test033_Foo_set_name_wrapped(test033_Foo_t* foo, char const* name, int* _result) {
    try {
        *_result = bblext::Foo_set_name_wrapped(*foo, name);
        return 0;
    } catch (std::exception& e) {
        _bbl_error_message = e.what();
        return 1;
    }
}

int test033_get_name_wrapped(char const** chars, size_t* len) {
    try {
        bblext::get_name_wrapped(chars, len);
        return 0;
    } catch (std::exception& e) {
        _bbl_error_message = e.what();
        return 1;
    }
}

} // extern "C"
