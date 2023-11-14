#pragma once

#include "bbl-capi.h"
#include "bbl-plugin.h"
#include "bbl-context.h"

#include <string>
#include <vector>

#ifdef _WIN32
#include <Windows.h>
#include <libloaderapi.h>
using PluginHandle =  HINSTANCE;
#endif

namespace bbl {

class Plugin {
    PluginHandle _handle;

    Plugin(Plugin const &) = delete;

public:
    Plugin(PluginHandle handle, std::string filename, std::string name, PluginExec fn_exec);
    Plugin(Plugin&& rhs);
    ~Plugin();

    std::string filename;
    std::string name;
    PluginExec fn_exec;
};

class PluginManager {
    std::vector<Plugin> _plugins;

public:
    PluginManager();

    void exec(bbl_context_t cpp_ctx, bbl_capi_t capi, char const* output_path);
};

}