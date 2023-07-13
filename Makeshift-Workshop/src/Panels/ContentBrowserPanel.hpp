#pragma once

#include "Makeshift/Renderer/Texture.hpp"

#include <imgui/imgui.h>

#include <filesystem>

namespace Makeshift {

	class ContentBrowserPanel {
	public:
		ContentBrowserPanel();
		
		void onImGuiRender();

	private:
		std::filesystem::path m_CurrentDirectory;

		Ref<Texture2D> m_DirectoryIcon;
		Ref<Texture2D> m_FileIcon;
	};

}
