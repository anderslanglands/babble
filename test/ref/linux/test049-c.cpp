
#include <set>
#include <string>

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

// This is necessary for some type definitions in lambdas defined in binding macros
namespace bbl { namespace detail {

template <typename T> struct argument_type;
template <typename T, typename U> struct argument_type<T(U)> {
    typedef U type;
};

} }


namespace bblext {

auto StringSetIterator_op_eq(const bbl::detail::argument_type<void (std::set<std::string>)>::type::iterator &l, const bbl::detail::argument_type<void (std::set<std::string>)>::type::iterator &r) -> bool {
    return l == r;
}

}


using test049_String_t = std::string;
using test049_StringSet_t = bbl::detail::argument_type<void (std::set<std::string>)>::type;
using test049_StringSetIterator_t = bbl::detail::argument_type<void (std::set<std::string>)>::type::iterator;

extern "C" {
int test049_String_dtor(test049_String_t* _this) {
    delete _this;
    return 0;
}

int test049_StringSet_empty(test049_StringSet_t const* _this, bool* _result) {
    *_result = _this->empty();
    return 0;
}

int test049_StringSet_size(test049_StringSet_t const* _this, size_t* _result) {
    *_result = _this->size();
    return 0;
}

int test049_StringSet_begin(test049_StringSet_t const* _this, test049_StringSetIterator_t** _result) {
    *_result = new bbl::detail::argument_type<void (std::set<std::string>)>::type::iterator(_this->begin());
    return 0;
}

int test049_StringSet_end(test049_StringSet_t const* _this, test049_StringSetIterator_t** _result) {
    *_result = new bbl::detail::argument_type<void (std::set<std::string>)>::type::iterator(_this->end());
    return 0;
}

int test049_StringSet_find(test049_StringSet_t* _this, test049_String_t const* __x, test049_StringSetIterator_t** _result) {
    try {
        *_result = new bbl::detail::argument_type<void (std::set<std::string>)>::type::iterator(_this->find(*__x));
        return 0;
    } catch (std::exception& e) {
        _bbl_error_message = e.what();
        return 1;
    }
}

int test049_StringSet_dtor(test049_StringSet_t* _this) {
    delete _this;
    return 0;
}

int test049_StringSetIterator_op_inc(test049_StringSetIterator_t* _this, test049_StringSetIterator_t** _result) {
    *_result = &_this->operator++();
    return 0;
}

int test049_StringSetIterator_op_mul(test049_StringSetIterator_t const* _this, test049_String_t const** _result) {
    *_result = &_this->operator*();
    return 0;
}

int test049_StringSetIterator_dtor(test049_StringSetIterator_t* _this) {
    delete _this;
    return 0;
}

int test049_StringSetIterator_op_eq(test049_StringSetIterator_t const* l, test049_StringSetIterator_t const* r, bool* _result) {
    try {
        *_result = bblext::StringSetIterator_op_eq(*l, *r);
        return 0;
    } catch (std::exception& e) {
        _bbl_error_message = e.what();
        return 1;
    }
}

} // extern "C"
