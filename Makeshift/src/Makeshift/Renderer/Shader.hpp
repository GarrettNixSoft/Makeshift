#pragma once

#include <string>
#include <unordered_map>

namespace Makeshift {

	class Shader {
	public:
		virtual ~Shader() {}

		virtual void bind() const = 0;
		virtual void unbind() const = 0;

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