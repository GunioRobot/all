#include "neural_network.hpp"
#include <iostream>

namespace Neural {

void Network::run() {
	const size_t n_neurons = neurons.size();
	for (size_t i = 0; i < n_neurons; ++i) {
		Neuron& neuron = neurons[i];
		neuron.cycle(threshold);
	}

	for (size_t i = 0; i < n_neurons; ++i) {
		Neuron& neuron = neurons[i];

		if (neuron.tofire) {
			neuron.fire();

			const size_t n_dendrites = neuron.dendrites.size();
			for (size_t j = 0; j < n_dendrites; ++j) {
				const Dendrite& dendrite = neuron.dendrites[j];
				const size_t idx = dendrite.getparent();
				Neuron& connected = neurons[idx];

				if (dendrite.inhibitory()) connected.inhibit();
				else connected.excite();
			}
		}
	}
}

void Network::load_adjacency_list(const std::vector< std::vector<Dendrite> >& adjacencylist) {
	const size_t n_neurons = adjacencylist.size();

	neurons.clear();
	neurons.resize(n_neurons);
	for (size_t i = 0; i < n_neurons; ++i) {
		Neuron& neuron = neurons[i];
		neuron.dendrites = adjacencylist[i];
	}
}
void Network::save(std::ostream& stream) {
	const size_t n_neurons = neurons.size();

	stream << n_neurons << ' ' << threshold;
	for (size_t i = 0; i < n_neurons; ++i) {
		const Neuron& neuron = neurons[i];
		const size_t n_dendrites = neuron.dendrites.size();

		stream << '\n' << neuron.sum << ' ' << neuron.tofire << ' ' << neuron.hasfired << ' ' << n_dendrites;
		for (size_t i = 0; i < n_dendrites; ++i) {
			const Dendrite& dendrite = neuron.dendrites[i];
			stream << ' ' << dendrite.parent << ' ' << dendrite.inhibitor;
		}
	}
}
void Network::load(std::istream& stream) {
	size_t n_neurons = 0;

	stream >> n_neurons >> threshold;
	neurons.resize(n_neurons);

	for (size_t i = 0; i < n_neurons; ++i) {
		Neuron& neuron = neurons[i];
		size_t n_dendrites = 0;

		stream >> neuron.sum >> neuron.tofire >> neuron.hasfired >> n_dendrites;
		neuron.dendrites.resize(n_dendrites, Dendrite(0));

		for (size_t i = 0; i < n_dendrites; ++i) {
			Dendrite& dendrite = neuron.dendrites[i];
			stream >> dendrite.parent >> dendrite.inhibitor;
		}
	}
}
}
