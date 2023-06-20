#pragma once

#include <memory>

#ifdef _WIN32
	#ifdef _WIN64
		#define MK_PLATFORM_WINDOWS
	#else
		#error "x86 builds are not supported!"
	#endif
#elif defined (__APPLE__) || defined (__MACH__)
	#include <TargetConditionals.h>
	#if TARGET_IPHONE_SIMULATOR == 1
		#error "iOS simulator is not supported!"
	#elif TARGET_OS_IPHONE == 1
		#define MK_PLATFORM_IOS
		#error "iOS is not supported!"
	#elif TARGET_OS_MAC == 1
		#define MK_PLATFORM_MACOS
		#error "MacOS is not supported!
	#else
		#error "Unknown Apple platform!"
	#endif
#elif defined (__ANDROID__)
	#define MK_PLATFORM_ANDROID
	#error "Android is not supported!"
#elif defined (__linux__)
	#define MK_PLATFORM_LINUX
	#error "Linux is not supported!"
#else
	#error "Unknown platform!"
#endif

#if MK_DYNAMIC_LINK
	#ifdef MK_BUILD_DLL
		#define MK_API __declspec(dllexport)
	#else
		#define MK_API __declspec(dllimport)
	#endif
#else
	#define MK_API
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
	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args) {
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args) {
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

}