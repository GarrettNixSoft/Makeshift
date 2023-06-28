#pragma once

#include <glm/glm.hpp>

namespace Makeshift {

	class Camera {
	public:
		Camera(const glm::mat4& projection) : m_Projection(projection) {}

		const glm::mat4& getProjection() const { return m_Projection; }

		// TODO: setPerspective, setOrtho, etc.
	private:
		glm::mat4 m_Projection;
	};

}