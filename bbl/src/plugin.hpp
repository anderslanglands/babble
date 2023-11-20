#pragma once

#include "bbl-capi.h"
#include "bbl-plugin.h"
#include "bbl-context.h"

#include <string>
#include <vector>
#include <unordered_map>

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

using PluginMap = std::unordered_map<std::string, Plugin>;

class PluginManager {
    PluginMap _plugins;

public:
    PluginManager();

    bool has_plugin(std::string const& name) {
        return _plugins.find(name) != _plugins.end();
    }

    PluginMap const& plugins() const {
        return _plugins;
    }
};

}