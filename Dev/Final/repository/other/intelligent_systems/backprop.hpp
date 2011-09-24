#pragma once

#include <cstdlib>
#include <cmath>
#include <vector>

namespace Neural {
template<size_t n_layers>
class Backprop {
	inline float activation(const float input) { return tanh(input); }
	inline float gradient(const float input) { return (1 - (input * input)); }

	std::vector<float> errors[n_layers - 1], outputs[n_layers - 1];
	std::vector< std::vector<float> > deltas[n_layers - 1], weights[n_layers - 1];

	const std::vector<size_t> config;
	const float bias;
public:
	Backprop(const std::vector<size_t> config, float bias) : config(config), bias(bias) {
		const size_t n_layers_ns = n_layers - 1;
		for (size_t i = 0;i < n_layers_ns; ++i) {
			const size_t i_s = i + 1; // i with input layer semantics (for config use only)

			const size_t n_layer_inputs = config[i_s - 1];
			const size_t n_layer_neurons = config[i_s];
			const size_t n_neuron_inputs = n_layer_inputs + 1; //+ 1 for bias

			outputs[i].resize(n_layer_neurons, 0);
			errors[i].resize(n_layer_neurons, 0);
			deltas[i].resize(n_layer_neurons);
			weights[i].resize(n_layer_neurons);

			for (size_t j = 0; j < n_layer_neurons; ++j) {
				deltas[i][j].resize(n_neuron_inputs, 0);
				weights[i][j].resize(n_neuron_inputs);

				for (size_t k = 0; k < n_neuron_inputs; ++k) {
					weights[i][j][k] = (0.5 - ((float)std::rand() / RAND_MAX)) * 2;
				}
			}
		}
	}

	const std::vector<float>& activate(const float input[]) {
		const size_t n_layers_ns = n_layers - 1;
		const size_t output_layer = n_layers_ns - 1;

		for (size_t i = 0;i < n_layers_ns; ++i) { // foreach layer
			const size_t i_s = i + 1; // i with input layer semantics (for config use only)

			const size_t lower_layer = i - 1;
			const size_t lower_layer_s = i_s - 1;

			const size_t n_layer_inputs = config[lower_layer_s];
			const size_t n_layer_neurons = config[i_s];

			const float* layer_input = i > 0 ? &outputs[lower_layer][0] : input;

			for (size_t j = 0;j < n_layer_neurons; ++j) { // foreach neuron
				//neuron_outputs[i][j] = layer_input[k] * weights[i][j][k];
				float sum = 0;

				for (size_t k = 0;k < n_layer_inputs; ++k) { // foreach weight
					sum += layer_input[k] * weights[i][j][k];
				}
				sum += bias * weights[i][j][n_layer_inputs];
				outputs[i][j] = activation(sum);
			}
		}
		return outputs[output_layer];
	}
	void train(const float input[], const float desired[], float learn_rate, float momentum) {
		activate(input);

		// calculated constants
		const float inverse_momentum = 1.0 - momentum;

		const size_t n_layers_ns = n_layers - 1;
		const size_t output_layer = n_layers_ns - 1;
		const size_t output_layer_s = n_layers - 1; ; // output_layer with input layer semantics (for config use only)

		const size_t n_outputs = config[output_layer_s];
		const size_t n_hidden_layers = output_layer - 1;

		const float* output_layer_input = output_layer > 0 ? &outputs[output_layer][0] : input;

		// calculate error for output layer
		for (size_t j = 0; j < n_outputs; ++j) {
			//errors[output_layer][j] = f'(outputs[i][j]) * (desired[j] - outputs[i][j]);
			errors[output_layer][j] = gradient(output_layer_input[j]) * (desired[j] - output_layer_input[j]);
		}

		// calculate error for hidden layers
		for (size_t i = n_hidden_layers; i <= n_hidden_layers; --i) { // i <= n_hidden_layers, unsigned numeric overflow
			const size_t i_s = i + 1; // i with input layer semantics (for config use only)
			const size_t upper_layer = i + 1;
			const size_t upper_layer_s = i_s + 1;

			const size_t n_layer_neurons = config[i_s];
			const size_t n_upperlayer_neurons = config[upper_layer_s];

			for (size_t j = 0; j < n_layer_neurons; ++j) {
				//error[i][j] = f'(upperlayer_output[j]) * SUMOF([upperlayer_error[k] * upperlayer_weights[k][j])
				float sum = 0;
				for (size_t k = 0; k < n_upperlayer_neurons; ++k) {
					sum += errors[upper_layer][k] * weights[upper_layer][k][j];
				}
				errors[i][j] = gradient(outputs[i][j]) * sum;
			}
		}

		// adjust weights for layers
		for (size_t i = 0; i < n_layers_ns; ++i) {
			const size_t i_s = i + 1; // i with input layer semantics (for config use only)
			const size_t lower_layer = i - 1;
			const size_t lower_layer_s = i_s - 1;

			const size_t n_layer_inputs = config[lower_layer_s];
			const size_t n_layer_neurons = config[i_s];

			const float* lower_layer_output = i > 0 ? &outputs[lower_layer][0] : input; // input layer semantics

			for (size_t j = 0; j < n_layer_neurons; ++j) {
				const float adjusted_error =  inverse_momentum * (learn_rate * errors[i][j]);

				for (size_t k = 0; k < n_layer_inputs; ++k) {
					// deltas[i][j][k] = (1 - momentum) * (learn_rate * errors[i][j]) * lowerlayer_outputs[k] + (momentum * deltas[i][j][k])
					const float delta = adjusted_error * lower_layer_output[k] + (momentum * deltas[i][j][k]);

					deltas[i][j][k] = delta;
					weights[i][j][k] += delta;
				}
				// bias
				const float delta = adjusted_error * bias + (momentum * deltas[i][j][n_layer_inputs]);

				deltas[i][j][n_layer_inputs] = delta;
				weights[i][j][n_layer_inputs] += delta;
			}
		}
	}
	void print() const;
};
}
