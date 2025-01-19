![example workflow](https://github.com/anderslanglands/babble/actions/workflows/build-and-test.yml/badge.svg)

# Usage

Download the release for your platform from the Releases page and unpack it somewhere. For building from source, see the instructions below.

## CMake Setup

Given a C++ library that we want to bind, the thing we need to generate is a C wrapper library that provides C types and function definitions that will call their corresponding C++ functions and methods.

In order to do that we use CMake to run `bbl-translate`, which takes a set of bind files as input and outputs C source code, then compiles the generated source into a library we can use.

A wrapper library's CMakeLists.txt looks like the below. This is the CMakeLists.txt for [bbl-usd](https://github.com/anderslanglands/bbl-usd) which binds [Pixar's OpenUSD](https://github.com/PixarAnimationStudios/OpenUSD).

```cmake
cmake_minimum_required(VERSION 3.15)
project(bbl-usd VERSION 0.1 LANGUAGES C CXX)

# babble should be found by adding the directory where you installed babble to
# the CMAKE_PREFIX_PATH variable
find_package(babble CONFIG REQUIRED)
find_package(pxr REQUIRED)

# The bindfile contains all our binding definitions
set(
  bindfiles
    bind/ar.cpp
    bind/gf.cpp
    bind/js.cpp
    bind/pcp.cpp
    bind/sdf.cpp
    bind/std.cpp
    bind/tf.cpp
    bind/usd/geom.cpp
    bind/usd/prim.cpp
    bind/usd/schema.cpp
    bind/usd/usd.cpp
    bind/vt.cpp
)

# bbl_translate_binding sets up the invocation of bbl-translate to generate the
# wrapper library source and creates the target called `usd-c`, which we can then
# add definitions, links etc to as a normal CMake library target.
# The first argument is the project name, from which the names of all the generated
# libraries will be created
# by appending the language name as `usd-c`, `usd-rust` etc.
bbl_translate_binding(
    openusd
    BINDFILES
        ${bindfiles}
    COMPILE_ARGS
        -Wno-deprecated-builtins
        -DNOMINMAX
        -D_MT
        -DBOOST_ALL_NO_LIB
        -D__TBB_show_deprecation_message_task_H
)

# bbl_translate_binding creates a library target in the form ${PROJECT_NAME}-c
# where PROJECT_NAME is the first argument to the function.
# We then add links, defs etc to the target as usual
target_link_libraries(openusd-c PUBLIC usd sdf js usdGeom)
target_compile_definitions(
  openusd-c
    PRIVATE
      NOMINMAX
      BOOST_ALL_NO_LIB
      __TBB_show_deprecation_message_task_H
)

if (MSVC)
    # OpenUSD is a pretty big library so we need bigobj on windows
    target_compile_options(openusd-c PRIVATE /bigobj)
endif()

# Compile a simple test program to exercise the generated library
add_executable(usd-c-test01 usd-c-test01.c)
target_link_libraries(usd-c-test01 PUBLIC openusd-c)
target_include_directories(usd-c-test01 PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
set_property(TARGET usd-c-test01 PROPERTY C_STANDARD 99)

# This is just here to trigger generation of compile commands for the bindfile so
# we get LSP functionality in the bind file. LSP completion of function names can
# be a huge time saver when authoring bindfiles.
add_library(bind-dummy ${bindfiles})
target_link_libraries(bind-dummy babble::bind)
target_include_directories(bind-dummy PRIVATE $<TARGET_PROPERTY:openusd-c,INCLUDE_DIRECTORIES>)
```

Then when CMake is ran as usual:

```sh
cmake -B build -G Ninja -DCMAKE_PREFIX_PATH="<path to babble>;<path to usd>;" -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```

The wrapper library will be built as well as in this case the C99 test program that uses it.

## Bindfiles

Bindfiles are C++ source files that contain definitions that allow `bbl-translate` to extract all the necessary information about types that are to be bound using clang. Their structure is somewhat similar to pybind, but they work very differently as instead of compiling an actual library that is then loaded in some other application, the bindfiles are just used to contstruct the AST in clang, and the AST is then translated to a C API by code generation.

### Simple Example

For example, given a simple C++ library:

```c++
namespace foo {
  /// \brief Very Barry
  class Bar {
  public:
    /// \brief set a baz value
    /// \param b the value to set
    void set_baz(float b);
    float get_baz() const;
  };
}
```

a bindfile for it would look like:

```c++
#include <babble> // required

BBL_MODULE(foo) {
  bbl::Class<foo::Bar>()
    .m(&foo::Bar::set_baz)
    .m(&foo::Bar::get_baz)
  ;
}
```

This would generate the following C API:

```c
/// \brief Vary Barry
typedef struct foo_Bar_t foo_Bar_t;
/// \brief set a baz value
/// \param b the value to set
int foo_Bar_set_baz(foo_Bar_t* _this, float b);
int foo_Bar_get_baz(foo_Bar_t const* _this, float* result);
```

Let's break down what's happening here piece by piece.

### `#include <babble>`

```c++
#include <babble>
```

The `babble` header contains the types we'll use to bind various different constructs, for example `bbl::Class`, and is required for all bindings.

### `BBL_MODULE`

```c++
BBL_MODULE(foo) {
```

This opens a new **module**, which in the case of a C library currently just specifies the prefix added to all constructs within that module. In our case we've defined a module `foo`, which means all the generated structs and functions are prefixed `foo_`. There can be multiple modules within one bindfile, and modules can also be split across multiple bindfiles.

Other language plugins may choose to do something more with modules (for example to split the generated libraries into multiple pieces).

### `bbl:Class` – Class Binding

```c++
bbl::Class<foo::Bar>()
```

This binds the class (or struct) `foo::Bar`, meaning that a C struct will be generated for it. It is required that any types referenced by bound methods or functions are themselves explicitly bound, or the function referencing them will be ignored.

#### Renaming

The `bbl::Class` constructor can optionally take a string argument that renames the generated struct. For instance, if we instead did:

```c++
bbl::Class<foo::Bar>("Baz")
```

The generated C would be:

```c
typedef struct foo_Baz_t foo_Baz_t;
int foo_Baz_set_baz(foo_Baz_t* _this, float b);
int foo_Baz_get_baz(foo_Baz_t const* _this, float* result);
```

This is useful in particular for renaming templated types, or removing common prefixes from type names that are redundant under the automatic module prefixing. For instance:

```c++
bbl::Class<std::vector<std::string>>("StringVector");
```

#### Binding Value Types

By default, babble will bind all types as *opaque pointers*. As can be seen in the example above, this means an undefined struct is created for the type and it is passed through the wrapper as a pointer, so the C code never knows anything about the type.

This is of course important because C++ contains many constructs that cannot be represented in C, so being able to see the innards of a `std::vector` makes no sense.

Sometimes though, you *can* represent a C++ type exactly in C and want to be able to poke at its fields directly. This is known as a *value type* in babble because they can be passed around by value on the stack. For instance:

```c++
// Target library header
namespace foo {

struct Point2D {
  float x;
  float y;
};

}
```

```c++
// bind file
BBL_MODULE(math) {
  bbl::Class<foo::Point2D>()
    .value_type()
    .f(&foo::Point2D::x)
    .f(&foo::Point2D::y)
  ;
}
```

```c
// generated header
struct math_Point2D_t {
  float x;
  float y;
};
```

Note that the `value_type()` method call on the `bbl::Class` binding tells babble that this type should be bound as a value type, and the fields are bound individually with the `f()` method call.

Currently, no checking is done to ensure that it is safe to bind types as value types, so it is up to you to make sure there are no sneaky move constructors or destructors that would cause undefined behaviour in the generated library.

Static asserts are generated in the wrapper library so that at least it can be ensured that the generated value type and the target C++ type have the same size and alignment, and the generated wrapper library will fail to compile if this is not the case.

#### Replacement Types

In some rare cases, a type *could be* a value type, but cannot be bound as such because the target library authors decided to make the fields private. For instance, if we change the declaration of `Point2D` from the previous example to a class instead of a struct, now all its fields are private and we can no longer bind it as a value type:

```c++
// Target library header
namespace foo {
class Point2D {
  float x; //< now private, value type binding will fail
  float y;
};
}
```

Now, the library authors *probably* had a good reason for doing this, but if you're sure it's safe to do so, you may wish to replace this type with an ABI-compatible struct that exposes those members directly to avoid a whole lot of `set_x()`/`get_y()` tomfoolery.

If you're *really, really sure* you can do this, then you achieve it like so:

```c++
// bindfile
struct MyPoint2D {
  float x;
  float y;
};

BBL_MODULE(math) {
  bbl::Class<foo::Point2D>()
    .replace_with<MyPoint2D>()
  ;
}
```

Which will generate the same `math_Point2D_t` struct as in the previous example.

#### Inheritance

Methods from superclasses are added to subclasses automatically when the superclasses are themselves bound (otherwise they cannot be extracted):

```c++
namespace foo {
  class Base {
  public:
    void do_base_thing();
  };

  class OtherBase {
  public:
    void do_other_base_thing();
  }

  class Derived : public Base, public OtherBase {
  public:
    void do_derived_thing();
  };
}
```

```c++
// bindfile
BBL_MODULE(foo) {
  bbl::Class<foo::Base>()
    .m(&foo::Base::do_base_thing)
  ;

  bbl::Class<foo::Derived>()
    .m(&foo::Derived::do_derived_thing)
  ;
}
```

```c
// generated header
typedef struct foo_Base_t foo_Base_t;
typedef struct foo_Derived_t foo_Derived_t;

int foo_Base_do_base_thing(foo_Base_t* _this);

int foo_Derived_do_base_thing(foo_Derived_t* _this);
int foo_Derived_do_derived_thing(foo_Derived_t* _this);
```

Note that `Derived`'s wrapper automatically picks up `do_base_thing()` from `Base` but not `do_other_base_thing()` form `OtherBase` because `Base` is bound but `OtherBase` is not.

#### Binding Smart Pointers

Consider:

```c++
namespace foo {
  class Bar;
  using BarPtr = std::unique_ptr<Bar>;
  using ConstBarPtr = std::unique_ptr<Bar const>;

  class Bar {
  public:
    static ConstBarPtr create();
    void set_baz(float b);
    float get_baz() const;
  };
}
```

One way to handle this would just be to bind the `get()` method on `std::unique_ptr` and then use the returned `Bar*` in client code. This can be a little long-winded though and encourages separating the type you're using (`Bar*`) from the type expressed in the target library (`BarPtr`), which makes it more likely you'll forget to call the destructor on `BarPtr`, leading to memory leaks and other shenanigans.

You can specify that a type is a smart pointer to another type using the `smartptr_to()` method on `bbl::Class`. This causes all methods from the "to" type to be added to the smart pointer type automatically:

```c++
// bindfile
BBL_MODULE(foo) {
  bbl::Class<foo::Bar>()
    .m(&foo::Bar::create)
    .m(&foo::Bar::set_baz)
    .m(&foo::Bar::get_baz)
  ;

  bbl::Class<foo::BarPtr>
    .smartptr_to<foo::Bar>()
  ;

  bbl::Class<foo::ConstBarPtr>
    .smartptr_to<foo::Bar const>()
  ;
}
```

```c
typedef struct foo_Bar_t foo_Bar_t;
typedef struct foo_BarPtr_t foo_BarPtr_t;
typedef struct foo_ConstBarPtr_t foo_ConstBarPtr_t;

int foo_Bar_create(foo_ConstBarPtr_t** result);

int foo_Bar_set_baz(foo_Bar_t* _this, float b);
int foo_Bar_get_baz(foo_Bar_t const* _this, float* result);

int foo_BarPtr_set_baz(foo_BarPtr_t* _this, float b);
int foo_BarPtr_get_baz(foo_BarPtr_t const* _this, float* result);

int foo_ConstBarPtr_get_baz(foo_ConstBarPtr_t const* _this, float* result);
```

Note that because `ConstBarPtr` was declared with a `const` template argument in `smartptr_to()`, it does not get the non-const methods from `Bar`.

Smart pointers also automatically receive methods from all the targeted type's superclasses if those superclasses are themselves bound.

### `.m()` – Method Binding

```c++
.m(&foo::Bar::set_baz)
```

The `m()` method on `bbl::Class` specifies a method to be bound and takes a member function pointer. From this, babble can extract everything it needs to know about the method in question, so a lot of the time, this is all you need to do.

Like the `bbl::Class` constructor, `m()` takes an optional second parameter that specifies a rename string for the method, which will rename the generated function in the C wrapper. For instance:

```c++
.m(&foo::Bar::set_baz, "SetQux")
```

would generate:

```c
int foo_Baz_SetQux(foo_Baz_t* _this, float b);
```

This is particularly useful in the case of overloaded methods (see below).

#### Exception Handling

The attentive reader will have noticed that the return value in `foo::Bar::get_baz()` has moved to an out parameter, and all generated functions return `int`. This is because all generated wrapper functions return an integer error code, and return values in C++ are always moved to out parameters.

The return codes are there in order to handle exceptions. The C wrapper will catch all exceptions and return a non-zero value in that case. Success is always indicated by a return value of zero.

If a function or method is marked as `noexcept` then the exception-handling machinery is not generated, but return values are still moved to out parameters for consistency's sake.

Currently, any caught exception will generate a return code of `1`, but future versions of babble will allow specifying unique error codes per exception type.

#### Overloaded Methods

Consider:

```c++
namespace foo {
  class Bar {
  public:
    void set_baz(float b);
    void set_baz(double b);
    float get_baz() const;
    double get_baz() const;
  };
}
```

Overloaded methods must have their pointers cast to the appropriate type in order to differentiate them, and you'll also want to specify a rename as well:

```c++
#include <babble> // required

BBL_MODULE(foo) {
  bbl::Class<foo::Bar>()
  .m((void (foo::Bar::*)(float))
    &foo::Bar::set_baz, "set_baz_float"
  )
  .m((void (foo::Bar::*)(double))
    &foo::Bar::set_baz, "set_baz_double"
  )
  .m((float (foo::Bar::*)() const)
    &foo::Bar::get_baz, "get_baz_float"
  )
  .m((double (foo::Bar::*)() const)
    &foo::Bar::get_baz, "get_baz_double"
  )
  ;
}
```

### Template Methods

Template methods must be bound separately for each type you wish to instantiate them with. For instance:

```c++
// target library header
namespace foo {
  class Bar {
  public:
    template <typename T>
    T get_baz() const;
  };
}
```

```c++
// bindfile
BBL_MODULE(foo) {
  bbl::Class<foo::Bar>()
  .m(&foo::Bar::get_baz<int>, "get_baz_int")
  .m(&foo::Bar::get_baz<float>, "get_baz_float")
  .m(&foo::Bar::get_baz<double>, "get_baz_double")
  ;
}
```

### Constructors

You cannot create a member function pointer to a constructor in C++, so they must be specified slightly differently:

```c++
// target library header
namespace foo {
  class Bar {
  public:
    Bar();
    Bar(float b);
  };
}
```

```c++
// bindfile
BBL_MODULE(foo) {
  bbl::Class<foo::Bar>()
    .ctor(bbl::Class<foo::Bar>::Ctor<>(), "default")
    .ctor(bbl::Class<foo::Bar>::Ctor<float>("value"), "from_float")
  ;
}
```

```c
// generated wrapper header
typedef struct foo_Bar_t foo_Bar_t;
int foo_Bar_default(foo_Bar_t** result);
int foo_Bar_from_float(float value, foo_Bar_t** result);
```

We bind constructors using the `ctor()` method on `bbl::Class`. The `ctor()` method takes an instance of `bbl::Class::Ctor`.

The template arguments to `bbl::Class::Ctor` define the types of the arguments to the constructor.

The parameters to the `bbl::Class::Ctor` constructor optionally define the names of the generated constructor parameters. These are optional and do not need to be the same as the parameter names in the target library, but if they are specified the number of names must match the number of arguments in the `Ctor` template argument list

Finally, the last argument to `ctor()` is an optional rename for the constructor function itself.

### Copy Constructors & Destructors

Copy constructors and destructors are automatically generated when appropriate. You do not need to do anything to enable this.

## `bbl::fn` – Binding Functions

Functions are bound in much the same way as methods, using the `bbl::fn` call:

```c++
// target library header
namespace foo {
  void do_thing();
}
```

```c++
// bindfile
BBL_MODULE(foo) {
  bbl::fn(&foo::do_thing)
}
```

```c
// generated wrapper header
int foo_do_thing();
```

As with the `m()` method on `bbl::Class`, `fn()` takes an optional second parameter that is the rename string for the function.

### Extension Functions

It is also possible to bind your own functions to be added to the generated wrapper library. Any functions declared in the `bblext` namespace can be bound as normal and will be treated as if they had been present in the target library. For instance:

```c++
// bindfile
namespace bblext {
  void my_do_thing(float bar) {
    std::cout << "doing something cool...\n";
  }
}

BBL_MODULE(foo) {
  bbl::fn(&bblext::my_do_thing, "do_thing");
}
```

```c
// generated wrapper source
int foo_do_thing(float bar) {
    std::cout << "doing something cool...\n";
    return 0;
}
```

This can be particularly useful for tweaking the interface of the target library to be more C-friendly. For instance, it is a common pattern in C++ for functions to take a `std::string const&` parameter:

```c++
// target library header
namespace foo {
  void hello(std::string const& msg);
}
```

```c++
// bindfile
BBL_MODULE(foo) {
  bbl::fn(&foo::hello);

  bbl::Class<std::string>("String")
    .ctor(bbl::Class<std::string>::Ctor<char const*>("str"), "from_c_str")
  ;
}
```

```c
// generated wrapper header
int foo_hello(foo_String_t const* msg);

typedef struct foo_String_t foo_String_t;
foo_String_from_c_str(char const* str, foo_String_t** result);
```

Using this API in client code then entails:

```c
foo_String_t* world;
foo_String_from_c_str("World!", &world);
foo_hello(world);
```

which is annoying, when what you really want to do is simply:

```c
foo_hello("World!");
```

We can provide this API by not binding `foo::hello()` directly, but by binding an extension function instead that just takes the `char const*` and calls `foo::hello()` with it:

```c++
// bindfile
namespace bblext {
  void hello(char const* msg) {
    foo::hello(msg);
  }
}

BBL_MODULE(foo) {
  bbl::fn(&bblext::hello);
}
```

which will generate:

```c
// generated wrapper header
foo_hello(char const* msg);
```

As of babble 0.5, it's possible to do this directly inline in the binding with...

## `bbl::Wrap` – Inline Lambda Wrappers

We can generate the exact same result as manually creating an extension function, but without having to write or bind that function by using `bbl::Wrap`.

Given a similar C++ target from the previous examples:

```c++
// target library header
namespace foo {
  void hello(std::string const& msg);

  struct Bar {
    void hello(std::string const& msg) const;
  };
}
```

we can do the replacement of `std::string const&` with `char const*` directly inline in the bindings using `bbl::Wrap`:

```c++
// bindfile
BBL_MODULE(foo) {
  bbl::fn(bbl::Wrap(&foo::hello, [](char const* msg) {
    foo::hello(msg);
  }));

  bbl::Class<foo::Bar>
    .m(bbl::Wrap(&foo::Bar::hello, [](foo::Bar const& bar, char const* msg) {
      bar.hello(msg);
    }), "hello_again");
  ;
}
```

## `bbl::Enum` – Binding Enums

Enums are bound similarly to classes:

```c++
// target library header
namespace foo {
  enum Axis {
    X = 0,
    Y,
    Z
  };
}
```

```c++
// bind file
BBL_MODULE(foo) {
  bbl::Enum<foo::Axis>();
}
```

```c
// generated wrapper header
typedef enum foo_Axis_t {
  foo_Axis_X = 0,
  foo_Axis_Y = 1,
  foo_Axis_Z = 2,
} foo_Axis_t;
```

### Enum Prefixes

By default, the enum's variants will be prefixed with the qualified name of the enum itself, to avoid collisions. If you want to change the prefix (or remove it), use the `prefix()` method on `bbl::Enum`:

```c++
// bind file
BBL_MODULE(foo) {
  bbl::Enum<foo::Axis>()
    .prefix("AXIS")
  ;
}
```

```c
// generated wrapper header
typedef enum foo_Axis_t {
  AXIS_X = 0,
  AXIS_Y = 1,
  AXIS_Z = 2,
} foo_Axis_t;
```

Passing an empty string to `prefix()` will remove the prefix completely.

## `std::function` Callbacks

Functions that take a callback in the form of a `std::function` can be bound by binding the `std::function` explicitly with `bbl::Class`. Consider this library that defines a class, `Foo` with member accessors, and another class `Bar`, which has a member `adjust_foo()` that takes a `FooFn` callback in order to modify an internal `Foo` object:

```c++
// target library header
namespace tst {
class Foo {
    int _a = 0;
public:

    void set_a(int a) {
        _a = a;
    }

    int get_a() const {
        return _a;
    }
};

using FooFn = std::function<Foo (Foo)>;

class Bar {
public:
    static void adjust_foo(FooFn fn) {
        Foo foo_before;
        Foo foo = fn(foo_before);
        printf("foo is now %d\n", foo.get_a());
    }
};
}
```

Here, `Bar::adjust_foo` takes a callback to modify the value of a `Foo` before printing it. This can be bound like so:

```c++
// bindfile
BBL_MODULE(tst) {
    bbl::Class<tst::Bar>("Bar")
        .m(tst::Bar::adjust_foo)
    ;

    // std::function bindings are just regular class bindings and don't need any extra setup
    bbl::Class<tst::FooFn>("FooFn");
    ;

    // we'll need these to actually modify Foo on the C side in the callback
    bbl::Class<tst::Foo>("Foo")
        .ctor(bbl::Class<tst::Foo>::Ctor<tst::Foo const&>("other"), "copy")
        .ctor(bbl::Class<tst::Foo>::Ctor<>(), "new")
        .m((tst::Foo& (tst::Foo::*)(tst::Foo const&))
            &tst::Foo::operator=, "op_eq")
        .m(&tst::Foo::set_a)
        .m(&tst::Foo::get_a)
    ;

}
```

Which generates the following wrapper API:

```c
typedef struct tst_Foo_t tst_Foo_t;
typedef struct tst_Bar_t tst_Bar_t;

int tst_Bar_adjust_foo(void (*fn)(tst_Foo_t* param00, tst_Foo_t** _result));

// ... snip
```

Note that the C version of `adjust_foo` takes a function pointer that matches the signature of the bound `std::function`, after having the same transformations applied as any other C++ function signature, e.g. the return parameter is moved to an out parameter.

The generated wrapper can be used in client code like so:

```c
// client code

// we can write a C callback matching the transformed signature
void fn(tst_Foo_t* foo, tst_Foo_t** result) {
    tst_Foo_copy(foo, result);
    tst_Foo_set_a(*result, 42);
}

int main() {
    // adjust_foo now takes the function pointer with the transformed signature
    tst_Bar_adjust_foo(fn);
    return 0;
}
```

Which will result in the following output when compiled and run:

```text
foo is now 42
```

Note that currently, only callbacks implemented as `std::function` can be handled this way. Regular function pointers do not work. In the future, limited support for function pointers will be added. The limitation will be that only function pointers whose signatures consist solely of builtins and value types can be bound.

## Subclassing C++ Types in C

TODO... implementation of this still to be done. It will work much the same as the `std::function` callbacks, but  passing a "virtual" function table to an automatically generated trampoline struct.

## `bbl::constant` – Binding Constants as `#defines`

TODO...

## Language Plugins

babble has a plugin architecture to allow generating wrappers for other languages that call the generated C bindings. This is currently WIP. See `plugins/rust` for an example.

## Using Babble as a Library

The target `babble::bbl` is a static, C library that can be used both to write new language plugins, as well as perform the same inspection on the target C++ and generated C APIs of a given target library. `include/bbl-context.h` and `include/bbl-capi.h` are the C API and `include/bbl-context.hpp` and `include/bbl-capi.hpp` are a C++ API wrapping the underlying C API.

See the `bbl-translate` source and the language plugins for how to use it.

TODO: proper docs

## Building `babble` From Source

### Prerequisites

#### `clang`

Requires at least clang 15. Tested with versions 15 through 17. Either download a binary distribution from the llvm-project releases page if one is available, or build from source (it's not hard, just loooong):

```sh
git clone https://github.com/llvm/llvm-project.git -b llvmorg-16.0.6
cd llvm-project
cmake -S llvm -B build -G Ninja \
  -DCMAKE_BUILD_TYPE=Release \
  -DLLVM_ENABLE_PROJECTS="clang;clang-tools-extra" \
  -DCMAKE_INSTALL_PREFIX=<llvm_install_path>

# ... go make a coffee
# ... if you have limited memory you'll want to lower the number of parallel jobs with e.g. -j2
cmake --build build --target install --config Release
```

### Build

There are some submodules so don't forget `--recursive` or `git submodule update --init` after the fact

I tend to use VSCode and the terminal for build/test cycle:

```sh
cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH=<llvm_install_path>
cmake --build build -j12 --config Release
```

On windows, the `RelWithDebInfo` config is set up for a debugging experience that doesn't make you want to jump out the window:
`cmake -B buildvs`

then open `buildvs/babble.sln` in VS, switch to `RelWithDebInfo` and update your debug working directory and arguments as per the testing section below

### Testing

Run the automated test suite with:

```sh
python ./run_tests.py
```

Run individual tests with:
`./build/bbl/bbl-translate test/test001.cpp -- -std=c++17 -Ibbl/include -- test001 -o build/test/out`
`./build/bbl/bbl-translate test/test002.cpp -- -std=c++17 -Ibbl/include -- test002 -o build/test/out`
...
`./build/bbl/bbl-translate test/test020.cpp -- -std=c++17 -Ibbl/include -- test020 -o build/test/out`
...etc.
