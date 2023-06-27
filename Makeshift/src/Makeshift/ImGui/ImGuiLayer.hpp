#pragma once

#include "Makeshift/Core/Layer.hpp"
#include "Makeshift/Events/KeyEvent.hpp"
#include "Makeshift/Events/MouseEvent.hpp"
#include "Makeshift/Events/ApplicationEvent.hpp"

namespace Makeshift {

	class MK_API ImGuiLayer : public Layer {
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void onAttach() override;
		virtual void onDetach() override;
		virtual void onEvent(Event& e) override;

		virtual void onImGuiRender(Timestep ts) {}

		void begin();
		void end();

		void blockEvents(bool block) { m_BlockEvents = block; }
	private:
		bool m_BlockEvents = true;
		float lastTime = 0.0f;
	};

}
