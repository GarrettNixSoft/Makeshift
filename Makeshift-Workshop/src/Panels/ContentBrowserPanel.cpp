#include "mkpch.hpp"
#include "ContentBrowserPanel.hpp"

#include <filesystem>

namespace Makeshift {

	// Change this when project system is implemented
	static std::filesystem::path s_AssetPath = "assets";

	ContentBrowserPanel::ContentBrowserPanel()
		: m_CurrentDirectory(s_AssetPath) {
		m_DirectoryIcon = Texture2D::Create("resources/icons/content_browser/directory_icon.png");
		m_FileIcon = Texture2D::Create("resources/icons/content_browser/file_icon.png");
	}

	void ContentBrowserPanel::onImGuiRender() {

		ImGui::Begin("Content Browser");

		// TO PARENT DIR
		if (m_CurrentDirectory != s_AssetPath) {
			if (ImGui::Button("..")) {
				m_CurrentDirectory = m_CurrentDirectory.parent_path();
			}
		}

		static float padding = 16.0f;
		static float thumbnailSize = 128;
		float cellSize = thumbnailSize + padding;

		float panelWidth = ImGui::GetContentRegionAvail().x;
		int columnCount = (int) (panelWidth / cellSize);
		columnCount = std::max(columnCount, 1);

		ImGui::Columns(columnCount, 0, false);

		// Iterate through assets directory
		for (auto& directoryEntry : std::filesystem::directory_iterator(m_CurrentDirectory)) {

			const auto& path = directoryEntry.path();
			auto relativePath = std::filesystem::relative(path, s_AssetPath);

			std::string relativePathString = relativePath.u8string();
			std::string filenameString = relativePath.filename().string();

			// DIR BUTTONS
			if (directoryEntry.is_directory()) {
				ImGui::ImageButton((ImTextureID)m_DirectoryIcon->getRendererId(), { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });
				if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
					m_CurrentDirectory /= path.filename();
				}
			}
			// FILE BUTTONS
			else {
				ImGui::ImageButton((ImTextureID)m_FileIcon->getRendererId(), { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });
				if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
					// TODO: Open asset file
				}
			}
			ImGui::TextWrapped(filenameString.c_str());

			ImGui::NextColumn();

		}

		ImGui::Columns(1);

		ImGui::SliderFloat("Thumbnail Size", &thumbnailSize, 64, 256);
		ImGui::SliderFloat("Padding", &padding, 0, 32);

		// TODO: status bar
		ImGui::End();

	}

}
