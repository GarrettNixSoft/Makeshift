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

#define BIT(x) (1 << x)