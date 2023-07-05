#pragma once

#include <string>

namespace Makeshift {

	class FileDialogs {
	public:
		// These return an empty string if dialog is cancelled
		static std::string OpenFile(const char* filter);
		static std::string SaveFile(const char* filter);
	};

}