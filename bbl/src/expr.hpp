#pragma once

#include <spdlog/fmt/fmt.h>

#include <memory>
#include <string>
#include <vector>

namespace bbl {

template <typename... Args>
std::string iformat(int indent, fmt::format_string<Args...> format_str,
                    Args&&... args) {
    return fmt::format("{:{}}{}", "", indent * 4,
                       fmt::format(format_str, std::forward<Args>(args)...));
}

struct Expr {
    virtual ~Expr() {}
    virtual std::string to_string(int depth) const = 0;
};

using ExprPtr = std::unique_ptr<Expr>;

struct ExprToken : public Expr {
    std::string token;

    ExprToken(std::string const& tok) : token(tok) {}

    static ExprPtr create(std::string const& tok) {
        return ExprPtr(new ExprToken{tok});
    }

    virtual std::string to_string(int depth) const override { return token; }
};

struct ExprReturn : public Expr {
    ExprPtr expr;

    ExprReturn(ExprPtr&& expr) : expr(std::move(expr)) {}

    static ExprPtr create(ExprPtr&& expr) {
        return ExprPtr(new ExprReturn{std::move(expr)});
    }

    virtual std::string to_string(int depth) const override {
        return fmt::format("return {}", expr->to_string(0));
    }
};

struct ExprCompound : public Expr {
    std::vector<ExprPtr> stmts;

    ExprCompound(std::vector<ExprPtr>&& stmts) : stmts(std::move(stmts)) {}

    static ExprPtr create(std::vector<ExprPtr>&& stmts) {
        return ExprPtr(new ExprCompound(std::move(stmts)));
    }

    virtual std::string to_string(int depth) const override {
        std::string result;
        for (auto const& stmt : stmts) {
            std::string line = iformat(depth, "{};\n", stmt->to_string(0));
            result = fmt::format("{}{}", result, line);
        }

        return result;
    }
};

struct ExprCall : public Expr {
    std::string name;
    std::vector<ExprPtr> params;

    ExprCall(std::string const& name, std::vector<ExprPtr>&& params)
        : name(name), params(std::move(params)) {}

    static ExprPtr create(std::string const& name,
                          std::vector<ExprPtr>&& params) {
        return ExprPtr(new ExprCall(name, std::move(params)));
    }

    virtual std::string to_string(int depth) const override {
        std::string result = iformat(depth, "{}(", name);

        bool first = true;

        for (auto const& param : params) {
            if (first) {
                first = false;
            } else {
                result = fmt::format("{}, ", result);
            }

            result = fmt::format("{}{}", result, param->to_string(0));
        }

        result = fmt::format("{})", result);
        return result;
    }
};

struct ExprDeref : public Expr {
    ExprPtr pointer;

    ExprDeref(ExprPtr&& pointer) : pointer(std::move(pointer)) {}

    static ExprPtr create(ExprPtr&& pointer) {
        return ExprPtr(new ExprDeref(std::move(pointer)));
    }

    virtual std::string to_string(int depth) const override {
        return iformat(depth, "*{}", pointer->to_string(0));
    }
};

struct ExprAddress : public Expr {
    ExprPtr pointee;

    ExprAddress(ExprPtr&& pointee) : pointee(std::move(pointee)) {}

    static ExprPtr create(ExprPtr&& pointee) {
        return ExprPtr(new ExprAddress(std::move(pointee)));
    }

    virtual std::string to_string(int depth) const override {
        return iformat(depth, "&{}", pointee->to_string(0));
    }
};

struct ExprAssign : public Expr {
    ExprPtr left;
    ExprPtr right;

    ExprAssign(ExprPtr&& left, ExprPtr&& right)
        : left(std::move(left)), right(std::move(right)) {}

    static ExprPtr create(ExprPtr&& left, ExprPtr&& right) {
        return ExprPtr(new ExprAssign(std::move(left), std::move(right)));
    }

    virtual std::string to_string(int depth) const override {
        return iformat(depth, "{} = {}", left->to_string(0),
                       right->to_string(0));
    }
};

struct ExprStaticCast : public Expr {
    ExprPtr cast_to;
    ExprPtr object;

    ExprStaticCast(ExprPtr&& cast_to, ExprPtr&& object)
        : cast_to(std::move(cast_to)), object(std::move(object)) {}

    static ExprPtr create(ExprPtr&& cast_to, ExprPtr&& object) {
        return ExprPtr(
            new ExprStaticCast(std::move(cast_to), std::move(object)));
    }

    virtual std::string to_string(int depth) const override {
        return iformat(depth, "static_cast<{}>({})", cast_to->to_string(0),
                       object->to_string(0));
    }
};

struct ExprArrow : public Expr {
    ExprPtr receiver;
    ExprPtr target;

    ExprArrow(ExprPtr&& receiver, ExprPtr&& target)
        : receiver(std::move(receiver)), target(std::move(target)) {}

    static ExprPtr create(ExprPtr&& receiver, ExprPtr&& target) {
        return ExprPtr(new ExprArrow(std::move(receiver), std::move(target)));
    }

    virtual std::string to_string(int depth) const override {
        return iformat(depth, "{}->{}", receiver->to_string(0),
                       target->to_string(0));
    }
};

struct ExprNamespaceRef : public Expr {
    std::string name;
    ExprPtr child;

    ExprNamespaceRef(std::string const& name, ExprPtr&& child)
        : name(name), child(std::move(child)) {}

    static ExprPtr create(std::string const& name, ExprPtr&& child) {
        return ExprPtr(new ExprNamespaceRef(name, std::move(child)));
    }

    virtual std::string to_string(int depth) const override {
        return iformat(depth, "{}::{}", name, child->to_string(0));
    }
};

struct ExprNew : public Expr {
    ExprPtr child;

    ExprNew(ExprPtr&& child) : child(std::move(child)) {}

    static ExprPtr create(ExprPtr&& child) {
        return ExprPtr(new ExprNew(std::move(child)));
    }

    virtual std::string to_string(int depth) const override {
        return iformat(depth, "new {}", child->to_string(0));
    }
};

struct ExprPlacementNew : public Expr {
    ExprPtr address;
    ExprPtr child;

    ExprPlacementNew(ExprPtr&& address, ExprPtr&& child)
        : address(std::move(address)), child(std::move(child)) {}

    static ExprPtr create(ExprPtr&& address, ExprPtr&& child) {
        return ExprPtr(
            new ExprPlacementNew(std::move(address), std::move(child)));
    }

    virtual std::string to_string(int depth) const override {
        return iformat(depth, "new ({}) {}", address->to_string(0),
                       child->to_string(0));
    }
};

struct ExprDelete : public Expr {
    ExprPtr child;

    ExprDelete(ExprPtr&& child) : child(std::move(child)) {}

    static ExprPtr create(ExprPtr&& child) {
        return ExprPtr(new ExprDelete(std::move(child)));
    }

    virtual std::string to_string(int depth) const override {
        return iformat(depth, "delete {}", child->to_string(0));
    }
};

struct Stmt : public Expr {
    ExprPtr pointer;

    Stmt(ExprPtr&& pointer) : pointer(std::move(pointer)) {}

    static ExprPtr create(ExprPtr&& pointer) {
        return ExprPtr(new Stmt(std::move(pointer)));
    }

    virtual std::string to_string(int depth) const override {
        return iformat(depth, "{};", pointer->to_string(0));
    }
};

} // namespace bbl