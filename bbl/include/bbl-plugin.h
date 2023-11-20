#ifndef BBL_PLUGIN_H
#define BBL_PLUGIN_H

#include "bbl-context.h"
#include "bbl-capi.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _WIN32
#define BBL_PLUGIN_API extern "C" __declspec(dllexport) 
#else 
#define BBL_PLUGIN_API extern "C"
#endif

#define BBL_PLUGIN_API_VERSION 1

typedef void (*PluginInit)(char const** name, int* version);
typedef int (*PluginExec)(bbl_context_t cpp_ctx, bbl_capi_t capi, char const* project_name, char const* output_path);


#ifdef __cplusplus
}
#endif

#endif