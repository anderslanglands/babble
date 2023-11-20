#include "bbl-context.h"
#include "bbl-detail.hpp"

#include <string_view>

namespace babble {

class Module {
    bbl_module_t _mod;

public:
    Module(bbl_module_t mod) : _mod(mod) {}

    auto name() const -> std::string_view {
        char const* ptr = nullptr;
        size_t len = 0;
        bbl_module_get_name(_mod, &ptr, &len);
        return {ptr, len};
    }

    auto num_classes() const -> size_t {
        size_t result = 0;
        bbl_module_num_classes(_mod, &result);
        return result;
    }
};

class ModuleIterator: public IndexIterator {
    bbl_context_t _ctx;

public:
    ModuleIterator(bbl_context_t ctx, size_t index) : _ctx(ctx), IndexIterator(index) {}

    auto operator*() const -> Module {
        bbl_module_t mod = nullptr;
        bbl_context_get_module(_ctx, _index, &mod);
        return Module(mod);
    }
};

using ModuleRange = Range<ModuleIterator>;

class Context {
    bbl_context_t _ctx;

public:
    Context(bbl_context_t ctx) : _ctx(ctx) {}

    auto num_modules() const -> size_t { 
        size_t result = 0;
        bbl_context_num_modules(_ctx, &result);
        return result;
    }

    auto get_module(size_t index) const -> Module {
        bbl_module_t mod = nullptr;
        bbl_context_get_module(_ctx, index, &mod);
        return Module(mod);
    }

    auto modules() const -> ModuleRange {
        return ModuleRange {
            ModuleIterator{_ctx, 0},
            ModuleIterator{_ctx, num_modules()}
        };
    }
};

}