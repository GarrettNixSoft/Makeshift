#pragma once

#include <imgui/imgui.h>

#include <filesystem>

namespace Makeshift {

	class ContentBrowserPanel {
	public:
		ContentBrowserPanel();
		
		void onImGuiRender();

	private:
		std::filesystem::path m_CurrentDirectory;
	};

}
