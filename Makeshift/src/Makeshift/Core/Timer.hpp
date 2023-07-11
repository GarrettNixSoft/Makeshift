#pragma once

#include <chrono>

namespace Makeshift {

	class Timer {
	public:
		Timer() { reset(); }

		void Timer::reset() {
			m_Start = std::chrono::high_resolution_clock::now();
		}

		float Timer::elapsed() {
			return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - m_Start).count() * 0.001f * 0.001f * 0.001f;
		}

		float Timer::elapsedMillis() {
			return elapsed() * 1000.0f;
		}

	private:
		std::chrono::time_point<std::chrono::high_resolution_clock> m_Start;
	};

}