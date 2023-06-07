#pragma once

#include "Makeshift/Window.hpp"
#include "Makeshift/Renderer/GraphicsContext.hpp"

#include <GLFW/glfw3.h>

namespace Makeshift {

	class WindowsWindow : public Window {
	public:
		WindowsWindow(const WindowProperties& properties);
		virtual ~WindowsWindow();

		void onUpdate() override;

		inline unsigned int getWidth() const override { return data.width; }
		inline unsigned int getHeight() const override { return data.height; }

		// Window attributes
		inline void setEventCallback(const EventCallbackFn& callback) override { data.eventCallback = callback; }
		void setVsync(bool enabled) override;
		bool isVsync() const override;

		inline virtual void* getNativeWindow() const { return window; }
	private:
		virtual void init(const WindowProperties& properties);
		virtual void shutdown();
	private:
		GLFWwindow* window;
		GraphicsContext* context;

		struct WindowData {
			std::string title;
			unsigned int width, height;
			bool vsync;

			EventCallbackFn eventCallback;
		};

		WindowData data;
	};

}
