#pragma once
#include "Core/Log.h"

typedef UINT uint;

//If it is under debug mode
//Allow asserts and Logging
#ifdef AT_DEBUG
#define AT_ENABLE_LOGGER
#define AT_ENABLE_ASSERTS
#endif

#ifdef AT_ENABLE_LOGGER
//ClientLogger Macros				 		
#define AT_TRACE(...)				::Atlas::Logger->Add("Trace", 0, "APP", __VA_ARGS__);
#define AT_INFO(...)				::Atlas::Logger->Add("Info", 1, "APP", __VA_ARGS__);
#define AT_WARN(...)				::Atlas::Logger->Add("Warning", 2, "APP", __VA_ARGS__);
#define AT_ERROR(...)				::Atlas::Logger->Add("Error", 3, "APP", __VA_ARGS__);
#define AT_CRITICAL(...)			::Atlas::Logger->Add("Critical", 4, "APP", __VA_ARGS__);
#else
//ClientLogger Macros			
#define AT_CRITICAL(...)		
#define AT_ERROR(...)			
#define AT_WARN(...)			
#define AT_INFO(...)			
#define AT_TRACE(...)			
#endif

//Attempt is a try catch macro
#define AT_ATTEMPT(x) {try{x;} catch(std::exception& e) {AT_CRITICAL("[File]:{1}\n[Line]:{2}\n[Description]:{3}", __FILE__,__LINE__, e.what()); }}

//Asserts break the debugger if a condition is false
//And output the error
#ifdef AT_ENABLE_ASSERTS
#define AT_ASSERT_WARG(x, string, ...) {if(!(x)) {AT_CRITICAL("Assertion Failed: " string, __VA_ARGS__); __debugbreak(); } }
#define AT_ASSERT(x, ...) {if(!(x)) {AT_CRITICAL("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
#define AT_ASSERT_WARG(x, ...) if(!(x)) throw std::exception();
#define AT_ASSERT(x, ...) if(!(x)) throw std::exception();
#endif