#include <memory>

#if defined(__GNUC__) || defined(__clang__)
#  define BBL_ALIGN(x) __attribute__ ((aligned(x)))
#elif defined(_MSC_VER)
#  define BBL_ALIGN(x) __declspec(align(x))
#else
#  error "Unknown compiler; can't define ALIGN"
#endif

#include <stddef.h>

extern "C" {

using test027_Foo_t = Foo;
using test027_FooPtr_t = FooPtr;
using test027_ConstFooPtr_t = ConstFooPtr;

int test027_Foo_create(test027_FooPtr_t** _result) {
    *_result = new FooPtr(Foo::create());
    return 0;
}

int test027_Foo_get_foo(test027_Foo_t const* _this, int* _result) {
    *_result = _this->get_foo();
    return 0;
}

int test027_Foo_set_foo(test027_Foo_t const* _this, int a, int* _result) {
    *_result = _this->set_foo(a);
    return 0;
}

int test027_Foo_dtor(test027_Foo_t* _this) {
    delete _this;
    return 0;
}

int test027_FooPtr_new(test027_FooPtr_t** _result) {
    *_result = new FooPtr();
    return 0;
}

int test027_FooPtr_dtor(test027_FooPtr_t* _this) {
    delete _this;
    return 0;
}

int test027_FooPtr_get_foo(test027_FooPtr_t const* _this, int* _result) {
    *_result = (*_this)->get_foo();
    return 0;
}

int test027_FooPtr_set_foo(test027_FooPtr_t const* _this, int a, int* _result) {
    *_result = (*_this)->set_foo(a);
    return 0;
}

int test027_ConstFooPtr_new(test027_ConstFooPtr_t** _result) {
    *_result = new ConstFooPtr();
    return 0;
}

int test027_ConstFooPtr_dtor(test027_ConstFooPtr_t* _this) {
    delete _this;
    return 0;
}

int test027_ConstFooPtr_get_foo(test027_ConstFooPtr_t const* _this, int* _result) {
    *_result = (*_this)->get_foo();
    return 0;
}

int test027_ConstFooPtr_set_foo(test027_ConstFooPtr_t const* _this, int a, int* _result) {
    *_result = (*_this)->set_foo(a);
    return 0;
}

} // extern "C"
