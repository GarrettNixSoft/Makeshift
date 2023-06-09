#pragma once

#include "mkpch.hpp"

#include "Makeshift/Core/Core.hpp"
#include "Makeshift/Events/Event.hpp"

namespace Makeshift {

	struct WindowProperties {
		std::string title;
		uint32_t width;
		uint32_t height;

		WindowProperties(const std::string& title = "Makeshift Engine",
						 uint32_t width = 1920,
						 uint32_t height = 1080)
			: title(title), width(width), height(height) {}
	};

	class MK_API Window {
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() {}

		virtual void onUpdate() = 0;

		virtual uint32_t getWidth() const = 0;
		virtual uint32_t getHeight() const = 0;

		// Window attributes
		virtual void setTitle(const std::string& title) = 0;
		virtual void setEventCallback(const EventCallbackFn& callback) = 0;
		virtual void setVsync(bool enabled) = 0;
		virtual bool isVsync() const = 0;

		virtual void* getNativeWindow() const = 0;

		static Scope<Window> create(const WindowProperties& properties = WindowProperties());
	};

}