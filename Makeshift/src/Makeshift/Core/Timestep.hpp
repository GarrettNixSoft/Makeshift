#pragma once

namespace Makeshift {

	class Timestep {
	public:
		Timestep(float t = 0.0f) : time(t) {}

		operator float() const { return time; }

		float getSeconds() const { return time; }
		float getMilliseconds() const { return time * 1000.0f; }

	private:
		float time;
	};

}