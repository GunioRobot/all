#pragma once

template<typename T>
class Accumulator {
	T m_accum;
	const T m_limit;
public:
	Accumulator(T limit) : m_accum(0), m_limit(limit) {}
	inline bool ready() const { return m_accum > m_limit; }

	inline void add(T dt) { m_accum += dt; }
	inline void reset() { m_accum = T(0); }
};
