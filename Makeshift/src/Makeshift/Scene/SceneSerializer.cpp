#include "mkpch.hpp"
#include "SceneSerializer.hpp"

#include "Entity.hpp"
#include "Components.hpp"

#include <fstream>
#include <yaml-cpp/yaml.h>

namespace YAML {

	template<>
	struct convert<glm::vec3> {

		static Node encode(const glm::vec3& rhs) {
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			return node;
		}

		static bool decode(const Node& node, glm::vec3& rhs) {
			if (!node.IsSequence() || node.size() != 3)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			return true;
		}

	};

	template<>
	struct convert<glm::vec4> {

		static Node encode(const glm::vec4& rhs) {
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			return node;
		}

		static bool decode(const Node& node, glm::vec4& rhs) {
			if (!node.IsSequence() || node.size() != 4)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();
			return true;
		}

	};

}

namespace Makeshift {

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v) {
		out << YAML::Flow; // [x, y, z]
		out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v) {
		out << YAML::Flow; // [x, y, z, w]
		out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
		return out;
	}

	SceneSerializer::SceneSerializer(const Ref<Scene>& scene) : m_Scene(scene) {

	}

	static void serializeEntity(YAML::Emitter& out, Entity entity) {

		out << YAML::BeginMap;
		out << YAML::Key << "Entity" << YAML::Value << "1234567890"; // TODO: entity UUID here

		if (entity.hasComponent<TagComponent>()) {
			out << YAML::Key << "TagComponent";
			out << YAML::BeginMap;

			auto& tag = entity.getComponent<TagComponent>().tag;
			out << YAML::Key << "Tag" << YAML::Value << tag;

			out << YAML::EndMap; // End of TagComponent map
		}

		if (entity.hasComponent<TransformComponent>()) {
			out << YAML::Key << "TransformComponent";
			out << YAML::BeginMap;

			auto& transform = entity.getComponent<TransformComponent>();
			out << YAML::Key << "Translation" << YAML::Value << transform.translation;
			out << YAML::Key << "Rotation" << YAML::Value << transform.rotation;
			out << YAML::Key << "Scale" << YAML::Value << transform.scale;

			out << YAML::EndMap; // End of TransformComponent map
		}

		if (entity.hasComponent<CameraComponent>()) {
			out << YAML::Key << "CameraComponent";
			out << YAML::BeginMap;

			auto& cameraComponent = entity.getComponent<CameraComponent>();
			auto& camera = cameraComponent.camera;

			out << YAML::Key << "Camera" << YAML::Value;
			out << YAML::BeginMap;
			out << YAML::Key << "ProjectionType" << YAML::Value << (int)camera.getProjectionType();
			out << YAML::Key << "PerspectiveFOV" << YAML::Value << camera.getPerspectiveVerticalFOV();
			out << YAML::Key << "PerspectiveNear" << YAML::Value << camera.getPerspectiveNearClip();
			out << YAML::Key << "PerspectiveFar" << YAML::Value << camera.getPerspectiveFarClip();
			out << YAML::Key << "OrthographicSize" << YAML::Value << camera.getOrthographicSize();
			out << YAML::Key << "OrthographicNear" << YAML::Value << camera.getOrthographicNearClip();
			out << YAML::Key << "OrthographicFar" << YAML::Value << camera.getOrthographicFarClip();
			out << YAML::EndMap; // End of Camera map

			out << YAML::Key << "Primary" << YAML::Value << cameraComponent.primary;
			out << YAML::Key << "FixedAspectRatio" << YAML::Value << cameraComponent.fixedAspectRatio;

			out << YAML::EndMap; // End of CameraComponent map

		}

		if (entity.hasComponent<SpriteRendererComponent>()) {
			out << YAML::Key << "SpriteRendererComponent";
			out << YAML::BeginMap;

			auto& spriteRendererComponent = entity.getComponent<SpriteRendererComponent>();
			out << YAML::Key << "Color" << YAML::Value << spriteRendererComponent.color;

			out << YAML::EndMap; // End of SpriteRendererComponent map
		}

		out << YAML::EndMap; // End of Entity map

	}

	void SceneSerializer::serialize(const std::string filepath) {

		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Scene" << YAML::Value << m_Scene->getName();
		out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;
		m_Scene->m_Registry.each([&](auto entityId) {
			Entity entity = { entityId, m_Scene.get() };
			if (!entity) return;

			serializeEntity(out, entity);
		});
		out << YAML::EndSeq;
		out << YAML::EndMap;

		std::ofstream fout(filepath);
		fout << "# Makeshift Engine Scene File\n" << out.c_str();

	}

	void SceneSerializer::serializeRuntime(const std::string filepath) {
		MK_CORE_ASSERT(false, "Not implemented yet"); // TODO

	}

	bool SceneSerializer::deserialize(const std::string filepath) {
		
		// open a file stream, read it into a string stream
		std::ifstream stream(filepath);
		std::stringstream strStream;
		strStream << stream.rdbuf();

		// load the string from the string stream and have the YAML lib parse it
		YAML::Node data = YAML::Load(strStream.str());

		// verify we have a Scene to read
		if (!data["Scene"])
			return false;

		// fetch the scene name and log it
		std::string sceneName = data["Scene"].as<std::string>();
		MK_CORE_TRACE("Deserializing scene '{0}'", sceneName);
		m_Scene->setName(sceneName);

		// fetch the entities in the scene, if present
		auto entities = data["Entities"];
		if (entities) {

			// iterate through all entities and parse their data/components
			for (auto entity : entities) {
				uint64_t uuid = entity["Entity"].as<uint64_t>(); // TODO

				// all entities have a TagComponent (name)
				std::string name;
				auto tagComponent = entity["TagComponent"];
				if (tagComponent)
					name = tagComponent["Tag"].as<std::string>();

				MK_CORE_TRACE("Deserialized entity with ID = {0}, name = {1}", uuid, name);

				Entity deserializedEntity = m_Scene->createEntity(name);

				// all entities have a TransformComponent
				auto transformComponent = entity["TransformComponent"];
				if (transformComponent) {
					// because the transform is always present, we can get it here instead of creating a new one
					auto& tc = deserializedEntity.getComponent<TransformComponent>();
					tc.translation = transformComponent["Translation"].as<glm::vec3>();
					tc.rotation = transformComponent["Rotation"].as<glm::vec3>();
					tc.scale = transformComponent["Scale"].as<glm::vec3>();
				}

				// ================================ OPTIONAL COMPONENTS ================================

				auto cameraComponent = entity["CameraComponent"];
				if (cameraComponent) {
					auto& cc = deserializedEntity.addComponent<CameraComponent>();

					auto& cameraProperties = cameraComponent["Camera"];
					cc.camera.setProjectionType((SceneCamera::ProjectionType)cameraProperties["ProjectionType"].as<int>());

					cc.camera.setPerspectiveVerticalFOV(cameraProperties["PerspectiveFOV"].as<float>());
					cc.camera.setPerspectiveNearClip(cameraProperties["PerspectiveNear"].as<float>());
					cc.camera.setPerspectiveFarClip(cameraProperties["PerspectiveFar"].as<float>());

					cc.camera.setOrthographicSize(cameraProperties["OrthographicSize"].as<float>());
					cc.camera.setOrthographicNearClip(cameraProperties["OrthographicNear"].as<float>());
					cc.camera.setOrthographicFarClip(cameraProperties["OrthographicFar"].as<float>());

					cc.primary = cameraComponent["Primary"].as<bool>();
					cc.fixedAspectRatio = cameraComponent["FixedAspectRatio"].as<bool>();
				}

				auto spriteRendererComponent = entity["SpriteRendererComponent"];
				if (spriteRendererComponent) {
					auto& src = deserializedEntity.addComponent<SpriteRendererComponent>();
					src.color = spriteRendererComponent["Color"].as<glm::vec4>();
				}

			}

		}

		// successfully loaded the scene
		return true;

	}

	bool SceneSerializer::deserializeRuntime(const std::string filepath) {
		MK_CORE_ASSERT(false, "Not implemented yet"); // TODO

		return false;
	}

}