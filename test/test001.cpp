#include "babble"

namespace qux {

/// A doc comment on the Foo class - brief
///
/// More detailed description here
/// ```
///     example code
/// ```
/// and some more detail
class Foo {
public:
    /// Comment on the method
    int bar(float a);
};

class Bar {
public:
    void baz(int b);
    char bum(short d);
};

class Bum {
public:
    void baz(int b);
    char bum(short d);
    float boo(float f);
};
}

// FunctionDecl bbl_bind
BBL_MODULE(test01) {
    // CompoundStmt
    // |
    // |-CXXFunctionalCastExpr bbl::Class<Foo>
    //   `-CXXConstructExpr bbl::Class<Foo>
    //     `-ImplicitCastExpr
    //       `-StringLiteral "Foo"
    bbl::Class<qux::Foo>("Foo").m(&qux::Foo::bar);

    /// XXX: These are explicitly not supported (yet)
    // |-DeclStmt
    //   `-VarDecl class_foo1
    //     `-CXXConstructExpr bbl::Class<Foo>
    //       `-ImplicitCastExpr
    //         `-StringLiteral "Foo"
    // bbl::Class<qux::Bar> class_bar("Bar");
    // class_bar.m(&qux::Bar::baz).m(&qux::Bar::bum);

    // |-DeclStmt
    //   `-VarDecl class_foo2
    //     `-ExprWithCleanups
    //       `-CXXConstructExpr
    //         `-MaterializeTemporaryExpr
    //           `-CXXFunctionalCastExpr
    //             `-CXXConstructExpr bbl::Class<Foo>
    //               `-ImplicitCastExpr
    //                 `-StringLiteral "Foo"
    auto class_bum = bbl::Class<qux::Bum>("Bum");
    class_bum
        .m(&qux::Bum::baz)
        .m(&qux::Bum::bum);
    class_bum
        .m(&qux::Bum::boo);
}
