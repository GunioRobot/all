#pragma once

#include <cstddef>
#include <cstdlib>

namespace GA {

/*class Agent {
	float fitness;
public:
	Agent();
	float getfitness() const;

	void crossover(const Agent& a, const Agent& b);
	void evaluate();
	void mutate(float mutation_amount);
};*/

template<class T, size_t N>
class Evolution {
	T gen1[N], gen2[N];
	bool gen2flag;

	float summed_error;

	bool addsum(float& sum, const float& r, size_t i) const {
		const T& candidate = !gen2flag ? gen2[i] : gen1[i]; // parents in alternate generation
		sum += candidate.getfitness();
		if (r < sum) return true;
		return false;
	}

	const T& findparent() const { // Weighted random, maximisation only
		const float r = ((float)std::rand() / RAND_MAX) * summed_error;
		float sum = 0;
		const T* altgen = !gen2flag ? gen2 : gen1; // parents in alternate generation
		for (size_t i = 0; i < N; ++i) {
			const T& candidate = altgen[i];
			if (addsum(sum, r, i)) return candidate;
		}
		return altgen[0];
	}
public:
	Evolution() : gen2flag(false) { evaluate(); }

	void evaluate() {
		summed_error = 0;
		T* gen = gen2flag ? gen2 : gen1;
		for (size_t i = 0; i < N; ++i) {
			T& evaluatee = gen[i];

			evaluatee.evaluate();
			summed_error += evaluatee.getfitness();
		}
	}

	const T& findelite() const {
		size_t idx = 0;
		float max = 0;

		const T* gen = gen2flag ? gen2 : gen1;
		for (size_t i = 0; i < N; ++i) {
			const T& candidate = gen[i];

			const float cfitness = candidate.getfitness();
			if (cfitness > max) {
				max = cfitness;
				idx = i;
			}
		}
		return gen[idx];
	}

	void run(float mutation_amount, float mutation_probability) {
		gen2flag = !gen2flag;

		T* gen = gen2flag ? gen2 : gen1;
		for (size_t i = 0; i < N; ++i) {
			T& child = gen[i];

			const T& a = findparent();
			const T& b = findparent();
			child.crossover(a,b);

			const float probability = (float)std::rand() / RAND_MAX;
			if (probability < mutation_probability) child.mutate(mutation_amount);
		}
		evaluate();
	}
};

}
