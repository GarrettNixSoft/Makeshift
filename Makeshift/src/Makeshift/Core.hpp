#pragma once

#include <memory>

#ifdef MK_PLATFORM_WINDOWS
#if MK_DYNAMIC_LINK
	#ifdef MK_BUILD_DLL
		#define MK_API __declspec(dllexport)
	#else
		#define MK_API __declspec(dllimport)
	#endif
#else
	#define MK_API
#endif
#else
	#error Makeshift currently only supports Windows!
#endif

#ifdef MK_DEBUG
	#define MK_ENABLE_ASSERTS
#endif

#ifdef MK_ENABLE_ASSERTS
	#define MK_ASSERT(x, ...) { if (!(x)) { MK_ERROR("Assertion failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define MK_CORE_ASSERT(x, ...) { if (!(x)) { MK_CORE_ERROR("Assertion failed: {0}"); __debugbreak(); } }
#else
	#define MK_ASSERT(x, ...)
	#define MK_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)

#define MK_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

namespace Makeshift {

	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T>
	using Ref = std::shared_ptr<T>;

}