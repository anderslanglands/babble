#pragma once

#include "bbl-capi.h"
#include "bbl-detail.hpp"

#include <string_view>
#include <optional>

namespace babble {

class C_Inclusion {
    bbl_capi_inclusion_t _inc;

public:
    C_Inclusion(bbl_capi_inclusion_t inc) : _inc(inc) {}

    auto get_directive() const -> std::string_view {
        char const* ptr = nullptr;
        size_t len = 0;
        bbl_capi_inclusion_get_directive(_inc, &ptr, &len);
        return {ptr, len};
    }

    auto get_filename() const -> std::string_view {
        char const* ptr = nullptr;
        size_t len = 0;
        bbl_capi_inclusion_get_filename(_inc, &ptr, &len);
        return {ptr, len};
    }

    auto is_local() const -> bool {
        bool is_local = false;
        bbl_capi_inclusion_is_local(_inc, &is_local);
        return is_local;
    }
};

class C_InclusionIterator: public IndexIterator {
    bbl_capi_module_t _mod;

public:
    C_InclusionIterator(bbl_capi_module_t mod, size_t index) : _mod(mod), IndexIterator(index) {}

    auto operator*() const -> C_Inclusion {
        bbl_capi_inclusion_t inc = nullptr;
        bbl_capi_module_get_inclusion(_mod, _index, &inc);
        return C_Inclusion(inc);
    }
};

using C_InclusionRange = Range<C_InclusionIterator>;

struct C_StructId {
    bbl_capi_structid_t id;
};

class C_StructIdIterator: public IndexIterator {
    bbl_capi_module_t _mod;

public:
    C_StructIdIterator(bbl_capi_module_t mod, size_t index) : _mod(mod), IndexIterator(index) {}

    auto operator*() const -> C_StructId {
        bbl_capi_structid_t id = nullptr;
        bbl_capi_module_get_struct(_mod, _index, &id);
        return C_StructId{id};
    }
};

struct C_StdFunctionId {
    bbl_capi_stdfunctionid_t id;
};

class C_StdFunctionIdIterator: public IndexIterator {
    bbl_capi_module_t _mod;

public:
    C_StdFunctionIdIterator(bbl_capi_module_t mod, size_t index) : _mod(mod), IndexIterator(index) {}

    auto operator*() const -> C_StdFunctionId {
        bbl_capi_stdfunctionid_t id = nullptr;
        bbl_capi_module_get_stdfunction(_mod, _index, &id);
        return C_StdFunctionId{id};
    }
};

using C_StdFunctionIdRange = Range<C_StdFunctionIdIterator>;

struct C_EnumVariant {
    std::string_view name;
    std::string_view value;
};

class C_EnumVariantIterator: public IndexIterator {
    bbl_capi_enum_t _enm;
public:
    C_EnumVariantIterator(bbl_capi_enum_t enm, size_t index) : _enm(enm), IndexIterator(index) {}

    auto operator*() const -> C_EnumVariant {
        char const* name = nullptr;
        size_t name_len = 0;
        char const* value = nullptr;
        size_t value_len = 0;
        bbl_capi_enum_get_variant(_enm, _index, &name, &name_len, &value, &value_len);
        return C_EnumVariant{std::string_view(name, name_len), std::string_view(value, value_len)};
    }
};

using C_EnumVariantRange = Range<C_EnumVariantIterator>;

class C_Enum {
    bbl_capi_enum_t _enm;

public:
    C_Enum(bbl_capi_enum_t enm) : _enm(enm) {}

    auto get_name() const -> std::string_view {
        char const* ptr = nullptr;
        size_t len = 0;
        bbl_capi_enum_get_name(_enm, &ptr, &len);
        return {ptr, len};
    }

    auto num_variants() const -> size_t {
        size_t result = 0;
        bbl_capi_enum_get_num_variants(_enm, &result);
        return result;
    }

    auto get_variant(size_t index) const -> C_EnumVariant {
        char const* name = nullptr;
        size_t name_len = 0;
        char const* value = nullptr;
        size_t value_len = 0;
        bbl_capi_enum_get_variant(_enm, index, &name, &name_len, &value, &value_len);
        return C_EnumVariant{std::string_view(name, name_len), std::string_view(value, value_len)};
    }

    auto variants() const -> C_EnumVariantRange {
        return C_EnumVariantRange{
            C_EnumVariantIterator{_enm, 0},
            C_EnumVariantIterator{_enm, num_variants()},
        };
    }

};

struct C_EnumId {
    bbl_capi_enumid_t id;
};

class C_EnumIdIterator: public IndexIterator {
    bbl_capi_module_t _mod;

public:
    C_EnumIdIterator(bbl_capi_module_t mod, size_t index) : _mod(mod), IndexIterator(index) {}

    auto operator*() const -> C_EnumId {
        bbl_capi_enumid_t id = nullptr;
        bbl_capi_module_get_enum(_mod, _index, &id);
        return C_EnumId{id};
    }
};

using C_EnumIdRange = Range<C_EnumIdIterator>;

class C_QType {
    bbl_capi_qtype_t _qt;

public:
    C_QType(bbl_capi_qtype_t qt): _qt(qt) {}

    auto is_const() const -> bool {
        bool is_const = false;
        bbl_capi_qtype_is_const(_qt, &is_const);
        return is_const;
    }

    auto get_type_kind() const -> bbl_capi_type_kind_t {
        bbl_capi_type_kind_t kind;
        bbl_capi_qtype_get_type_kind(_qt, &kind);
        return kind;
    }

    auto get_as_builtin() const -> std::optional<bbl_builtin_t> {
        if (get_type_kind() == bbl_capi_typekind_Builtin) {
            bbl_builtin_t builtin;
            bbl_capi_qtype_get_as_builtin(_qt, &builtin);
            return builtin;
        } else {
            return {};
        }
    }

    auto get_as_structid() const -> std::optional<C_StructId> {
        if (get_type_kind() == bbl_capi_typekind_Struct) {
            bbl_capi_structid_t structid;
            bbl_capi_qtype_get_as_structid(_qt, &structid);
            return C_StructId{structid};
        } else {
            return {};
        }
    }

    auto get_as_stdfunctionid() const -> std::optional<C_StdFunctionId> {
        if (get_type_kind() == bbl_capi_typekind_StdFunction) {
            bbl_capi_stdfunctionid_t stdfunctionid;
            bbl_capi_qtype_get_as_stdfunctionid(_qt, &stdfunctionid);
            return C_StdFunctionId{stdfunctionid};
        } else {
            return {};
        }
    }

    auto get_as_enumid() const -> std::optional<C_EnumId> {
        if (get_type_kind() == bbl_capi_typekind_Enum) {
            bbl_capi_enumid_t enumid;
            bbl_capi_qtype_get_as_enumid(_qt, &enumid);
            return C_EnumId{enumid};
        } else {
            return {};
        }
    }

    auto get_pointee_type() const -> std::optional<C_QType> {
        if (get_type_kind() == bbl_capi_typekind_Pointer) {
            bbl_capi_qtype_t pointee_type;
            bbl_capi_qtype_get_pointee_type(_qt, &pointee_type);
            return C_QType(pointee_type);
        } else {
            return {};
        }
    }

    auto get_array_element_type() const -> std::optional<C_QType> {
        if (get_type_kind() == bbl_capi_typekind_Array) {
            bbl_capi_qtype_t array_element_type;
            bbl_capi_qtype_get_array_element_type(_qt, &array_element_type);
            return C_QType(array_element_type);
        } else {
            return {};
        }
    }

    auto get_array_element_size() const -> std::optional<size_t> {
        if (get_type_kind() == bbl_capi_typekind_Array) {
            size_t size;
            bbl_capi_qtype_get_array_size(_qt, &size);
            return size;
        } else {
            return {};
        }
    }
};

class C_Param {
    bbl_capi_param_t _param;

public:
    C_Param(bbl_capi_param_t param): _param(param) {}

    auto get_name() const -> std::string_view {
        char const* ptr = nullptr;
        size_t len = 0;
        bbl_capi_param_get_name(_param, &ptr, &len);
        return {ptr, len};
    }

    auto get_type() -> const C_QType {
        bbl_capi_qtype_t qt;
        bbl_capi_param_get_type(_param, &qt);
        return C_QType(qt);
    }
};

class C_ParamIterator: public IndexIterator {
    bbl_capi_function_t _fun;
public:
    C_ParamIterator(bbl_capi_function_t fun, size_t index) : _fun(fun), IndexIterator(index) {}

    auto operator*() const -> C_Param {
        bbl_capi_param_t param = nullptr;
        bbl_capi_function_get_param(_fun, _index, &param);
        return C_Param{param};
    }
};

using C_ParamRange = Range<C_ParamIterator>;

class C_StdFunctionParamIterator: public IndexIterator {
    bbl_capi_stdfunction_t _fun;
public:
    C_StdFunctionParamIterator(bbl_capi_stdfunction_t fun, size_t index) : _fun(fun), IndexIterator(index) {}

    auto operator*() const -> C_Param {
        bbl_capi_param_t param = nullptr;
        bbl_capi_stdfunction_get_param(_fun, _index, &param);
        return C_Param{param};
    }
};

using C_StdFunctionParamRange = Range<C_StdFunctionParamIterator>;

class C_Field {
    bbl_capi_field_t _field;

public:
    C_Field(bbl_capi_field_t field): _field(field) {}

    auto get_name() const -> std::string_view {
        char const* ptr = nullptr;
        size_t len = 0;
        bbl_capi_field_get_name(_field, &ptr, &len);
        return {ptr, len};
    }

    auto get_type() -> const C_QType {
        bbl_capi_qtype_t qt;
        bbl_capi_field_get_type(_field, &qt);
        return C_QType(qt);
    }
};

class C_FieldIterator: public IndexIterator {
    bbl_capi_struct_t _strct;
public:
    C_FieldIterator(bbl_capi_struct_t strct, size_t index) : _strct(strct), IndexIterator(index) {}

    auto operator*() const -> C_Field {
        bbl_capi_field_t field = nullptr;
        bbl_capi_struct_get_field(_strct, _index, &field);
        return C_Field{field};
    }
};

using C_FieldRange = Range<C_FieldIterator>;

class C_Struct {
    bbl_capi_struct_t _strct;

public:
    C_Struct(bbl_capi_struct_t strct) : _strct(strct) {}

    auto get_name() const -> std::string_view {
        char const* ptr = nullptr;
        size_t len = 0;
        bbl_capi_struct_get_name(_strct, &ptr, &len);
        return {ptr, len};
    }
    
    auto get_bind_kind() const -> bbl_bind_kind_t {
        bbl_bind_kind_t bind_kind;
        bbl_capi_struct_get_bind_kind(_strct, &bind_kind);
        return bind_kind;
    }

    auto num_fields() const -> size_t {
        size_t result = 0;
        bbl_capi_struct_get_num_fields(_strct, &result);
        return result;
    }

    auto get_field(size_t index) const -> C_Field {
        bbl_capi_field_t id = nullptr;
        bbl_capi_struct_get_field(_strct, index, &id);
        return C_Field{id};
    }

    auto fields() const -> C_FieldRange {
        return C_FieldRange {
            C_FieldIterator{_strct, 0},
            C_FieldIterator{_strct, num_fields()},
        };
    }
};

using C_StructIdRange = Range<C_StructIdIterator>;

class C_Function {
    bbl_capi_function_t _fun;

public:
    C_Function(bbl_capi_function_t fun) : _fun(fun) {}

    auto get_name() const -> std::string_view {
        char const* ptr = nullptr;
        size_t len = 0;
        bbl_capi_function_get_name(_fun, &ptr, &len);
        return {ptr, len};
    }

    auto get_result_param() const -> std::optional<C_Param> {
        bbl_capi_param_t param = nullptr;
        bbl_capi_function_get_result_param(_fun, &param);
        if (param == nullptr) {
            return {};
        } else {
            return C_Param(param);
        }
    }

    auto get_receiver_param() const -> std::optional<C_Param> {
        bbl_capi_param_t param = nullptr;
        bbl_capi_function_get_receiver_param(_fun, &param);
        if (param == nullptr) {
            return {};
        } else {
            return C_Param(param);
        }
    }

    auto num_params() const -> size_t {
        size_t result = 0;
        bbl_capi_function_get_num_params(_fun, &result);
        return result;
    }

    auto get_param(size_t index) const -> C_Param {
        bbl_capi_param_t id = nullptr;
        bbl_capi_function_get_param(_fun, index, &id);
        return C_Param{id};
    }

    auto params() const -> C_ParamRange {
        return C_ParamRange {
            C_ParamIterator{_fun, 0},
            C_ParamIterator{_fun, num_params()},
        };
    }
};

struct C_FunctionId {
    bbl_capi_functionid_t id;
};

class C_FunctionIdIterator: public IndexIterator {
    bbl_capi_module_t _mod;

public:
    C_FunctionIdIterator(bbl_capi_module_t mod, size_t index) : _mod(mod), IndexIterator(index) {}

    auto operator*() const -> C_FunctionId {
        bbl_capi_functionid_t id = nullptr;
        bbl_capi_module_get_function(_mod, _index, &id);
        return C_FunctionId{id};
    }
};

using C_FunctionIdRange = Range<C_FunctionIdIterator>;

class C_StdFunction {
    bbl_capi_stdfunction_t _fun;

public:
    C_StdFunction(bbl_capi_stdfunction_t fun) : _fun(fun) {}

    auto get_name() const -> std::string_view {
        char const* ptr = nullptr;
        size_t len = 0;
        bbl_capi_stdfunction_get_name(_fun, &ptr, &len);
        return {ptr, len};
    }

    auto get_result_param() const -> std::optional<C_Param> {
        bbl_capi_param_t param = nullptr;
        bbl_capi_stdfunction_get_result_param(_fun, &param);
        if (param == nullptr) {
            return {};
        } else {
            return C_Param(param);
        }
    }

    auto num_params() const -> size_t {
        size_t result = 0;
        bbl_capi_stdfunction_get_num_params(_fun, &result);
        return result;
    }

    auto get_param(size_t index) const -> C_Param {
        bbl_capi_param_t id = nullptr;
        bbl_capi_stdfunction_get_param(_fun, index, &id);
        return C_Param{id};
    }

    auto params() const -> C_StdFunctionParamRange {
        return C_StdFunctionParamRange {
            C_StdFunctionParamIterator{_fun, 0},
            C_StdFunctionParamIterator{_fun, num_params()},
        };
    }
};


class C_Module {
    bbl_capi_module_t _mod;

public:
    C_Module(bbl_capi_module_t mod) : _mod(mod) {}

    auto num_inclusions() const -> size_t {
        size_t result = 0;
        bbl_capi_module_get_num_inclusions(_mod, &result);
        return result;
    }

    auto get_inclusion(size_t index) const -> C_Inclusion {
        bbl_capi_inclusion_t inc = nullptr;
        bbl_capi_module_get_inclusion(_mod, index, &inc);
        return C_Inclusion(inc);
    }

    auto inclusions() const -> C_InclusionRange {
        return C_InclusionRange {
            C_InclusionIterator{_mod, 0},
            C_InclusionIterator{_mod, num_inclusions()},
        };
    }

    auto num_structs() const -> size_t {
        size_t result = 0;
        bbl_capi_module_get_num_structs(_mod, &result);
        return result;
    }

    auto get_struct(size_t index) const -> C_StructId {
        bbl_capi_structid_t id = nullptr;
        bbl_capi_module_get_struct(_mod, index, &id);
        return C_StructId{id};
    }

    auto structs() const -> C_StructIdRange {
        return C_StructIdRange {
            C_StructIdIterator{_mod, 0},
            C_StructIdIterator{_mod, num_structs()},
        };
    }

    auto num_functions() const -> size_t {
        size_t result = 0;
        bbl_capi_module_get_num_functions(_mod, &result);
        return result;
    }

    auto get_function(size_t index) const -> C_FunctionId {
        bbl_capi_functionid_t id = nullptr;
        bbl_capi_module_get_function(_mod, index, &id);
        return C_FunctionId{id};
    }

    auto functions() const -> C_FunctionIdRange {
        return C_FunctionIdRange {
            C_FunctionIdIterator{_mod, 0},
            C_FunctionIdIterator{_mod, num_functions()},
        };
    }

    auto num_stdfunctions() const -> size_t {
        size_t result = 0;
        bbl_capi_module_get_num_stdfunctions(_mod, &result);
        return result;
    }

    auto get_stdfunction(size_t index) const -> C_StdFunctionId {
        bbl_capi_stdfunctionid_t id = nullptr;
        bbl_capi_module_get_stdfunction(_mod, index, &id);
        return C_StdFunctionId{id};
    }

    auto stdfunctions() const -> C_StdFunctionIdRange {
        return C_StdFunctionIdRange {
            C_StdFunctionIdIterator{_mod, 0},
            C_StdFunctionIdIterator{_mod, num_stdfunctions()},
        };
    }

    auto num_enums() const -> size_t {
        size_t result = 0;
        bbl_capi_module_get_num_enums(_mod, &result);
        return result;
    }

    auto get_enum(size_t index) const -> C_EnumId {
        bbl_capi_enumid_t id = nullptr;
        bbl_capi_module_get_enum(_mod, index, &id);
        return C_EnumId{id};
    }

    auto enums() const -> C_EnumIdRange {
        return C_EnumIdRange {
            C_EnumIdIterator{_mod, 0},
            C_EnumIdIterator{_mod, num_enums()},
        };
    }

};

class C_ModuleIterator : public IndexIterator {
    bbl_capi_t _capi;

public:
    C_ModuleIterator(bbl_capi_t capi, size_t index)
        : _capi(capi), IndexIterator(index) {}

    auto operator*() const -> C_Module {
        bbl_capi_module_t mod = nullptr;
        bbl_capi_get_module(_capi, _index, &mod);
        return C_Module(mod);
    }
};

using C_ModuleRange = Range<C_ModuleIterator>;

class C_API {
    bbl_capi_t _capi;

public:
    C_API(bbl_capi_t capi) : _capi(capi) {}

    auto get_header() -> std::string_view {
        char const* ptr = nullptr;
        size_t len = 0;
        bbl_capi_get_header(_capi, &ptr, &len);
        return {ptr, len};
    }

    auto get_source() -> std::string_view {
        char const* ptr = nullptr;
        size_t len = 0;
        bbl_capi_get_header(_capi, &ptr, &len);
        return {ptr, len};
    }

    auto num_modules() const -> size_t {
        size_t result = 0;
        bbl_capi_num_modules(_capi, &result);
        return result;
    }

    auto get_module(size_t index) const -> C_Module {
        bbl_capi_module_t mod = nullptr;
        bbl_capi_get_module(_capi, index, &mod);
        return C_Module(mod);
    }

    auto modules() const -> C_ModuleRange {
        return C_ModuleRange {
            C_ModuleIterator{_capi, 0},
            C_ModuleIterator{_capi, num_modules()},
        };
    }

    auto get_function(C_FunctionId id) const -> C_Function {
        bbl_capi_function_t fun = nullptr;
        bbl_capi_get_function(_capi, id.id, &fun);
        return C_Function(fun);
    }

    auto get_struct(C_StructId id) const -> C_Struct {
        bbl_capi_struct_t strct = nullptr;
        bbl_capi_get_struct(_capi, id.id, &strct);
        return C_Struct(strct);
    }

    auto get_enum(C_EnumId id) const -> C_Enum {
        bbl_capi_enum_t enm = nullptr;
        bbl_capi_get_enum(_capi, id.id, &enm);
        return C_Enum(enm);
    }

    auto get_stdfunction(C_StdFunctionId id) const -> C_StdFunction {
        bbl_capi_stdfunction_t fun = nullptr;
        bbl_capi_get_stdfunction(_capi, id.id, &fun);
        return C_StdFunction(fun);
    }

};

} // namespace babble