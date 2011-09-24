#pragma once

#include <iomanip>
#include <vector>

namespace Neural {

class Network;

class Dendrite {
	size_t parent;
	bool inhibitor;

	inline size_t getparent() const { return parent; }
	inline bool inhibitory() const { return inhibitor; }
public:
	Dendrite(size_t parent, bool inhibitor = false) : parent(parent), inhibitor(inhibitor) {}

	friend class Network;
};

class Neuron {
	int sum;
	bool tofire, hasfired;

	std::vector<Dendrite> dendrites;

	inline void excite() { ++sum; }
	inline void fire() { sum = -sum; tofire = false; hasfired = true; }
	inline void inhibit() { --sum; }

	inline void cycle(int threshold) {
		if (sum >= threshold) tofire = true;
		else if (sum > 0) --sum;
		else if (sum < 0) ++sum;
		if (hasfired) hasfired = false;
	}

	inline bool fired() const { return hasfired; }
public:
	Neuron() : sum(0), tofire(false), hasfired(false) {}

	friend class Network;
};

class Network {
	int threshold;
	std::vector<Neuron> neurons;


public:
	Network(size_t n_neurons, int threshold) : threshold(threshold), neurons(n_neurons) {}

	inline void fire(size_t i) {
		Neuron& neuron = neurons[i];
		neuron.sum = threshold;
	}
	void run();

	inline bool hasfired(size_t i) const {
		const Neuron& neuron = neurons[i];
		return neuron.fired();
	}

	void load_adjacency_list(const std::vector< std::vector<Dendrite> >& adjacencylist);

	void save(std::ostream& stream);
	void load(std::istream& stream);
};
}
