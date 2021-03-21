#pragma once
#ifdef ATLASLIB
#include "Core/Log.h"
#include "Graphics/DX11Exception.h"

typedef UINT uint;

//If it is under debug mode
//Allow asserts and Logging
#ifdef AT_DEBUG
#define AT_ENABLE_LOGGER
#define AT_ENABLE_ASSERTS
#endif

#ifdef AT_ENABLE_LOGGER
//CoreLogger macros
#define AT_CORE_TRACE(...)			::Atlas::Log::Logger->Add("Trace", 0, "ATLAS", __VA_ARGS__);
#define AT_CORE_INFO(...)			::Atlas::Log::Logger->Add("Info", 1,"ATLAS", __VA_ARGS__);
#define AT_CORE_WARN(...)			::Atlas::Log::Logger->Add("Warning", 2, "ATLAS", __VA_ARGS__);
#define AT_CORE_ERROR(...)			::Atlas::Log::Logger->Add("Error", 3, "ATLAS", __VA_ARGS__);
#define AT_CORE_CRITICAL(...)		::Atlas::Log::Logger->Add("Critical", 4, "ATLAS", __VA_ARGS__);
#else
//CoreLogger macros
#define AT_CORE_CRITICAL(...)
#define AT_CORE_ERROR(...)		
#define AT_CORE_WARN(...)		
#define AT_CORE_INFO(...)		
#define AT_CORE_TRACE(...)		
#endif

//Attempt is a try catch macro
#define AT_CORE_ATTEMPT(x) {try{x;} catch(std::exception& e) { AT_CORE_CRITICAL("[File]:{0}\n[Line]:{1}\n[Description]:{2}", __FILE__,__LINE__, e.what()); }}

//Asserts break the debugger if a condition is false
//And output the error
#ifdef AT_ENABLE_ASSERTS
#define	AT_CORE_ASSERT_WARG(x, string, ...) {if(!(x)) {AT_CORE_CRITICAL("Assertion Failed: " string, __VA_ARGS__); __debugbreak(); } }
#define	AT_CORE_ASSERT(x, ...) {if(!(x)) {AT_CORE_CRITICAL("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
#define	AT_CORE_ASSERT_WARG(x, ...) if(!(x)) throw std::exception();
#define	AT_CORE_ASSERT(x, ...) if(!(x)) throw std::exception();
#endif
#endif