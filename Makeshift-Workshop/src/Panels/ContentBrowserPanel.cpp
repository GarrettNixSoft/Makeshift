#include "mkpch.hpp"
#include "ContentBrowserPanel.hpp"

#include <filesystem>

namespace Makeshift {

	// Change this when project system is implemented
	extern const std::filesystem::path g_AssetPath = "assets";

	ContentBrowserPanel::ContentBrowserPanel()
		: m_CurrentDirectory(g_AssetPath) {
		m_DirectoryIcon = Texture2D::Create("resources/icons/content_browser/directory_icon.png");
		m_FileIcon = Texture2D::Create("resources/icons/content_browser/file_icon.png");
	}

	void ContentBrowserPanel::onImGuiRender() {

		ImGui::Begin("Content Browser");

		// TO PARENT DIR
		if (m_CurrentDirectory != g_AssetPath) {
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
			auto relativePath = std::filesystem::relative(path, g_AssetPath);

			std::string filenameString = relativePath.filename().string();
			ImGui::PushID(filenameString.c_str());

			Ref<Texture2D> icon = directoryEntry.is_directory() ? m_DirectoryIcon : m_FileIcon;

			// Button Style
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4());

			ImGui::ImageButton((ImTextureID)icon->getRendererId(), { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });

			if (ImGui::BeginDragDropSource()) {
				const wchar_t* itemPath = relativePath.c_str();				// Add 1 here for null termination
				ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t), ImGuiCond_Once);
				ImGui::EndDragDropSource();
			}

			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
				// DIR BUTTON LOGIC
				if (directoryEntry.is_directory()) {
					m_CurrentDirectory /= path.filename();
				}
				// FILE BUTTON lOGIC
				else {
					// TODO: Open asset file
				}
			}

			// Pop Button Style
			ImGui::PopStyleColor();

			// File/Directory Name Label
			ImGui::TextWrapped(filenameString.c_str());

			ImGui::NextColumn();

			ImGui::PopID();

		}

		ImGui::Columns(1);

		ImGui::SliderFloat("Thumbnail Size", &thumbnailSize, 64, 256);
		ImGui::SliderFloat("Padding", &padding, 0, 32);

		// TODO: status bar
		ImGui::End();

	}

}
