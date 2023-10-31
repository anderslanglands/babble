#pragma once

#include <spdlog/fmt/fmt.h>

#include <memory>
#include <string>
#include <vector>

namespace bbl {

template <typename... Args>
std::string
iformat(int indent, fmt::format_string<Args...> format_str, Args&&... args) {
    return fmt::format("{:{}}{}",
                       "",
                       indent * 4,
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

inline ExprPtr ex_token(std::string const& tok) {
    return ExprPtr(new ExprToken{tok});
}

struct ExprReturn : public Expr {
    ExprPtr expr;

    ExprReturn(ExprPtr&& expr) : expr(std::move(expr)) {}

    virtual std::string to_string(int depth) const override {
        return fmt::format("return {}", expr->to_string(0));
    }
};

inline ExprPtr ex_return(ExprPtr&& expr) {
    return ExprPtr(new ExprReturn{std::move(expr)});
}

struct ExprCompound : public Expr {
    std::vector<ExprPtr> stmts;

    ExprCompound(std::vector<ExprPtr>&& stmts) : stmts(std::move(stmts)) {}

    virtual std::string to_string(int depth) const override {
        std::string result;
        for (auto const& stmt : stmts) {
            std::string line = iformat(depth, "{};\n", stmt->to_string(0));
            result = fmt::format("{}{}", result, line);
        }

        return result;
    }
};

inline ExprPtr ex_compound(std::vector<ExprPtr>&& stmts) {
    return ExprPtr(new ExprCompound(std::move(stmts)));
}

struct ExprParameterList : public Expr {
    std::vector<ExprPtr> params;

    ExprParameterList(std::vector<ExprPtr>&& params)
        : params(std::move(params)) {}

    virtual std::string to_string(int depth) const override {
        std::string result = iformat(depth, "(");

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

inline std::unique_ptr<ExprParameterList>
ex_parameter_list(std::vector<ExprPtr>&& params) {
    return std::make_unique<ExprParameterList>(std::move(params));
}

struct ExprCall : public Expr {
    std::string name;
    std::unique_ptr<ExprParameterList> params;

    ExprCall(std::string const& name,
             std::unique_ptr<ExprParameterList>&& params)
        : name(name), params(std::move(params)) {}

    virtual std::string to_string(int depth) const override {
        return iformat(depth, "{}{}", name, params->to_string(0));
    }
};

inline ExprPtr ex_call(std::string const& name,
                       std::unique_ptr<ExprParameterList>&& params) {
    return ExprPtr(new ExprCall(name, std::move(params)));
}

struct ExprParen : public Expr {
    ExprPtr inner;

    ExprParen(ExprPtr&& inner) : inner(std::move(inner)) {}

    virtual std::string to_string(int depth) const override {
        return iformat(depth, "({})", inner->to_string(0));
    }
};

inline ExprPtr ex_paren(ExprPtr&& inner) {
    return ExprPtr(new ExprParen(std::move(inner)));
}

struct ExprDeref : public Expr {
    ExprPtr pointer;

    ExprDeref(ExprPtr&& pointer) : pointer(std::move(pointer)) {}

    virtual std::string to_string(int depth) const override {
        return iformat(depth, "*{}", pointer->to_string(0));
    }
};

inline ExprPtr ex_deref(ExprPtr&& pointer) {
    return ExprPtr(new ExprDeref(std::move(pointer)));
}

struct ExprAddress : public Expr {
    ExprPtr pointee;

    ExprAddress(ExprPtr&& pointee) : pointee(std::move(pointee)) {}

    virtual std::string to_string(int depth) const override {
        return iformat(depth, "&{}", pointee->to_string(0));
    }
};

inline ExprPtr ex_address(ExprPtr&& pointee) {
    return ExprPtr(new ExprAddress(std::move(pointee)));
}

struct ExprMove : public Expr {
    ExprPtr pointee;

    ExprMove(ExprPtr&& pointee) : pointee(std::move(pointee)) {}

    virtual std::string to_string(int depth) const override {
        return iformat(depth, "std::move({})", pointee->to_string(0));
    }
};

inline ExprPtr ex_move(ExprPtr&& pointee) {
    return ExprPtr(new ExprMove(std::move(pointee)));
}

struct ExprAssign : public Expr {
    ExprPtr left;
    ExprPtr right;

    ExprAssign(ExprPtr&& left, ExprPtr&& right)
        : left(std::move(left)), right(std::move(right)) {}

    virtual std::string to_string(int depth) const override {
        return iformat(
            depth, "{} = {}", left->to_string(0), right->to_string(0));
    }
};

inline ExprPtr ex_assign(ExprPtr&& left, ExprPtr&& right) {
    return ExprPtr(new ExprAssign(std::move(left), std::move(right)));
}

struct ExprStaticCast : public Expr {
    ExprPtr cast_to;
    ExprPtr object;

    ExprStaticCast(ExprPtr&& cast_to, ExprPtr&& object)
        : cast_to(std::move(cast_to)), object(std::move(object)) {}

    virtual std::string to_string(int depth) const override {
        return iformat(depth,
                       "static_cast<{}>({})",
                       cast_to->to_string(0),
                       object->to_string(0));
    }
};

inline ExprPtr ex_static_cast(ExprPtr&& cast_to, ExprPtr&& object) {
    return ExprPtr(new ExprStaticCast(std::move(cast_to), std::move(object)));
}

struct ExprReinterpretCast : public Expr {
    ExprPtr cast_to;
    ExprPtr object;

    ExprReinterpretCast(ExprPtr&& cast_to, ExprPtr&& object)
        : cast_to(std::move(cast_to)), object(std::move(object)) {}

    virtual std::string to_string(int depth) const override {
        return iformat(depth,
                       "reinterpret_cast<{}>({})",
                       cast_to->to_string(0),
                       object->to_string(0));
    }
};

inline ExprPtr ex_reinterpret_cast(ExprPtr&& cast_to, ExprPtr&& object) {
    return ExprPtr(
        new ExprReinterpretCast(std::move(cast_to), std::move(object)));
}

struct ExprArrow : public Expr {
    ExprPtr receiver;
    ExprPtr target;

    ExprArrow(ExprPtr&& receiver, ExprPtr&& target)
        : receiver(std::move(receiver)), target(std::move(target)) {}

    virtual std::string to_string(int depth) const override {
        return iformat(
            depth, "{}->{}", receiver->to_string(0), target->to_string(0));
    }
};

inline ExprPtr ex_arrow(ExprPtr&& receiver, ExprPtr&& target) {
    return ExprPtr(new ExprArrow(std::move(receiver), std::move(target)));
}

struct ExprNamespaceRef : public Expr {
    std::string name;
    ExprPtr child;

    ExprNamespaceRef(std::string const& name, ExprPtr&& child)
        : name(name), child(std::move(child)) {}

    virtual std::string to_string(int depth) const override {
        return iformat(depth, "{}::{}", name, child->to_string(0));
    }
};

inline ExprPtr ex_namespace_ref(std::string const& name, ExprPtr&& child) {
    return ExprPtr(new ExprNamespaceRef(name, std::move(child)));
}

struct ExprNew : public Expr {
    ExprPtr child;

    ExprNew(ExprPtr&& child) : child(std::move(child)) {}

    virtual std::string to_string(int depth) const override {
        return iformat(depth, "new {}", child->to_string(0));
    }
};

inline ExprPtr ex_new(ExprPtr&& child) {
    return ExprPtr(new ExprNew(std::move(child)));
}

struct ExprPlacementNew : public Expr {
    ExprPtr address;
    ExprPtr child;

    ExprPlacementNew(ExprPtr&& address, ExprPtr&& child)
        : address(std::move(address)), child(std::move(child)) {}

    virtual std::string to_string(int depth) const override {
        return iformat(
            depth, "new ({}) {}", address->to_string(0), child->to_string(0));
    }
};

inline ExprPtr ex_placement_new(ExprPtr&& address, ExprPtr&& child) {
    return ExprPtr(new ExprPlacementNew(std::move(address), std::move(child)));
}

struct ExprVarDecl : public Expr {
    ExprPtr type;
    ExprPtr name;

    ExprVarDecl(ExprPtr&& type, ExprPtr&& name)
        : type(std::move(type)), name(std::move(name)) {}

    virtual std::string to_string(int depth) const override {
        return iformat(depth, "{} {}", type->to_string(0), name->to_string(0));
    }
};

inline ExprPtr ex_var_decl(ExprPtr&& type, ExprPtr&& name) {
    return ExprPtr(new ExprVarDecl(std::move(type), std::move(name)));
}

struct ExprDelete : public Expr {
    ExprPtr child;

    ExprDelete(ExprPtr&& child) : child(std::move(child)) {}

    virtual std::string to_string(int depth) const override {
        return iformat(depth, "delete {}", child->to_string(0));
    }
};

inline ExprPtr ex_delete(ExprPtr&& child) {
    return ExprPtr(new ExprDelete(std::move(child)));
}

struct Stmt : public Expr {
    ExprPtr pointer;

    Stmt(ExprPtr&& pointer) : pointer(std::move(pointer)) {}

    virtual std::string to_string(int depth) const override {
        return iformat(depth, "{};", pointer->to_string(0));
    }
};

inline ExprPtr ex_stmt(ExprPtr&& pointer) {
    return ExprPtr(new Stmt(std::move(pointer)));
}

struct ExprFunPtrWrapperLambda : public Expr {
    std::string name;
    std::string type;
    ExprPtr parameter_list;
    ExprPtr body_compound;

    ExprFunPtrWrapperLambda(std::string const& name,
                            std::string const& type,
                            std::vector<ExprPtr> parameter_list,
                            std::vector<ExprPtr> body_compound)
        : name(name), type(type),
          parameter_list(ex_parameter_list(std::move(parameter_list))),
          body_compound(ex_compound(std::move(body_compound))) {}

    virtual std::string to_string(int depth) const override {
        return iformat(depth,
                       "{} {} = [&]{} {{\n{}    }}",
                       type,
                       name,
                       parameter_list->to_string(0),
                       body_compound->to_string(depth + 2));
    }
};

inline ExprPtr ex_fun_wrapper_lambda(std::string const& name,
                                     std::string const& type,
                                     std::vector<ExprPtr> parameter_list,
                                     std::vector<ExprPtr> body_compound) {
    return ExprPtr(new ExprFunPtrWrapperLambda(
        name, type, std::move(parameter_list), std::move(body_compound)));
}

} // namespace bbl