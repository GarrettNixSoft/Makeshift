#pragma once

namespace Makeshift {

	struct EditorContext {

		bool sceneEdited = false;

		std::string activeSceneFilePath = std::string();

		std::function<void(void)> editCallback;

		std::function<void(void)> saveCallback;

		void flagEdit() {
			if (!sceneEdited) {
				sceneEdited = true;
				editCallback();
			}
		}

		void clearEdit() {
			if (sceneEdited) {
				sceneEdited = false;
				saveCallback();
			}

		}

	};

}