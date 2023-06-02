#pragma once

#ifdef MK_PLATFORM_WINDOWS
	#ifdef MK_BUILD_DLL
		#define MK_API __declspec(dllexport)
	#else
		#define MK_API __declspec(dllimport)
	#endif
#else
	#error Makeshift currently only supports Windows!
#endif

#ifdef MK_ENALE_ASSERTS
	#define MK_ASSERT(x, ...) { if (!(x)) { MK_ERROR("Assertion failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define MK_CORE_ASSERT(x, ...) { if (!(x)) { MK_CORE_ERROR("Assertion failed: {0}"); __debugbreak(); } }
#else
	#define MK_ASSERT(x, ...)
	#define MK_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)

#define MK_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)