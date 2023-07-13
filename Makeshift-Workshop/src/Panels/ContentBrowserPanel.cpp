#include "mkpch.hpp"
#include "ContentBrowserPanel.hpp"

#include <filesystem>

namespace Makeshift {

	// Change this when project system is implemented
	static std::filesystem::path s_AssetPath = "assets";

	ContentBrowserPanel::ContentBrowserPanel()
		: m_CurrentDirectory(s_AssetPath) {
		
	}

	void ContentBrowserPanel::onImGuiRender() {

		ImGui::Begin("Content Browser");

		// TO PARENT DIR
		if (m_CurrentDirectory != s_AssetPath) {
			if (ImGui::Button("..")) {
				m_CurrentDirectory = m_CurrentDirectory.parent_path();
			}
		}

		// Iterate through assets directory
		for (auto& directoryEntry : std::filesystem::directory_iterator(m_CurrentDirectory)) {

			const auto& path = directoryEntry.path();
			auto relativePath = std::filesystem::relative(path, s_AssetPath);

			std::string relativePathString = relativePath.u8string();
			std::string filenameString = relativePath.filename().string();

			// DOWN DIR BUTTONS
			if (directoryEntry.is_directory()) {
				if (ImGui::Button(filenameString.c_str())) {
					
					m_CurrentDirectory /= path.filename();

				}
			}
			else {
				if (ImGui::Button(filenameString.c_str())) {

				}
			}
			

		}

		ImGui::End();

	}

}
