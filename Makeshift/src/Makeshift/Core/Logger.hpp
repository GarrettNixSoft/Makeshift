#pragma once

// ================ INCLUDES ================
// -------- Engine --------
#include "Core.hpp"
// -------- Library --------
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/fmt/ostr.h>
// -------- Standard --------
#include <cassert>
// ================ INCLUDES ================

namespace Makeshift {

	class MK_API Logger {

	public:
		static void init();

		inline static std::shared_ptr<spdlog::logger>& getCoreLogger() { return coreLogger; }
		inline static std::shared_ptr<spdlog::logger>& getClientLogger() { return clientLogger; }

	private:
		static std::shared_ptr<spdlog::logger> coreLogger;
		static std::shared_ptr<spdlog::logger> clientLogger;
	};

}

// Macros for Core Logging
#define MK_CORE_FATAL(...) ::Makeshift::Logger::getCoreLogger()->fatal(__VA_ARGS__)
#define MK_CORE_ERROR(...) ::Makeshift::Logger::getCoreLogger()->error(__VA_ARGS__)
#define MK_CORE_WARN(...)  ::Makeshift::Logger::getCoreLogger()->warn(__VA_ARGS__)
#define MK_CORE_INFO(...)  ::Makeshift::Logger::getCoreLogger()->info(__VA_ARGS__)
#define MK_CORE_DEBUG(...) ::Makeshift::Logger::getCoreLogger()->debug(__VA_ARGS__)
#define MK_CORE_TRACE(...) ::Makeshift::Logger::getCoreLogger()->trace(__VA_ARGS__)

// Macros for Client Logging
#define MK_FATAL(...) ::Makeshift::Logger::getClientLogger()->fatal(__VA_ARGS__)
#define MK_ERROR(...) ::Makeshift::Logger::getClientLogger()->error(__VA_ARGS__)
#define MK_WARN(...)  ::Makeshift::Logger::getClientLogger()->warn(__VA_ARGS__)
#define MK_INFO(...)  ::Makeshift::Logger::getClientLogger()->info(__VA_ARGS__)
#define MK_DBG(...) ::Makeshift::Logger::getClientLogger()->debug(__VA_ARGS__)
#define MK_TRACE(...) ::Makeshift::Logger::getClientLogger()->trace(__VA_ARGS__)