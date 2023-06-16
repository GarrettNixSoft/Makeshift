#pragma once

#include <string>
#include <unordered_map>

#include <glm/glm.hpp>

namespace Makeshift {

	class Shader {
	public:
		virtual ~Shader() {}

		virtual void bind() const = 0;
		virtual void unbind() const = 0;

		virtual void setFloat(const std::string& name, float value) = 0;
		virtual void setInt(const std::string& name, int value) = 0;
		virtual void setBool(const std::string& name, int value) = 0;

		virtual void setVec2(const std::string& name, const glm::vec2& value) = 0;
		virtual void setVec3(const std::string& name, const glm::vec3& value) = 0;
		virtual void setVec4(const std::string& name, const glm::vec4& value) = 0;

		virtual void setMat3(const std::string& name, const glm::mat3& value) = 0;
		virtual void setMat4(const std::string& name, const glm::mat4& value) = 0;

		virtual const std::string& getName() const = 0;

		static Ref<Shader> Create(const std::string& filePath);
		static Ref<Shader> Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
	};

	class ShaderLibrary {
	public:
		void add(const Ref<Shader>& shader);
		void add(const std::string& name, const Ref<Shader>& shader);
		Ref<Shader> load(const std::string& filePath); // use file name as shader name
		Ref<Shader> load(const std::string& name, const std::string& filePath); // custom name

		Ref<Shader> get(const std::string& name);

		bool exists(const std::string& name) const;
	private:
		std::unordered_map<std::string, Ref<Shader>> shaders;
	};

}