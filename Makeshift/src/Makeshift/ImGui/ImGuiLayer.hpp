#pragma once

#include "Makeshift/Layer.hpp"

namespace Makeshift {

	class MK_API ImGuiLayer : public Layer {
	public:
		ImGuiLayer();
		~ImGuiLayer();

		void onAttach();
		void onDetach();
		void onUpdate();
		void onEvent(Event& event);
	private:
		float lastTime = 0.0f;
	};

}
