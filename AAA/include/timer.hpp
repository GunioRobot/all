#pragma once

class Timer {
	double m_previous, m_delta;
public:
	Timer() : m_previous(0), m_delta(0) {}

	static double getTime();

	inline double delta() const { return m_delta; }
	inline void update() {
		const double current = Timer::getTime();

		m_delta = current - m_previous;
		m_previous = current;
	}
};
