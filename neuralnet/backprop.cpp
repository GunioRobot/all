#include <backprop.h>

#include <dmath.h>
#include <stdlib.h>

inline double activation(double input) { return tanh(input); }
inline double gradient(double input) { return (1 - (input * input)); }

Backprop::Backprop(const unsigned int config[], const unsigned int n_layers, const double bias, const double learn_rate, const double momentum)	: bias(bias), learn_rate(learn_rate), momentum(momentum), n_layers(n_layers) {
	this->config = new unsigned int[n_layers];
	for (unsigned int i = 0;i < n_layers; ++i) {
		this->config[i] = config[i];
	}

	const unsigned int n_layers_ns = n_layers - 1; // n_layers non simulated, ie without input layer semantics

	outputs = new double*[n_layers_ns]; // [hidden, output] : [0,1]
	errors = new double*[n_layers_ns]; // [hidden, output] : [0,1]
	deltas = new double**[n_layers_ns]; // [hidden, output] : [0,1]
	weights = new double**[n_layers_ns]; // [hidden, output] : [0,1]

	for (unsigned int i = 0;i < n_layers_ns; ++i) {
		const unsigned int i_s = i + 1; // i with input layer semantics (for config use only)

		const unsigned int n_layer_inputs = config[i_s - 1];
		const unsigned int n_layer_neurons = config[i_s];
		const unsigned int n_neuron_inputs = n_layer_inputs + 1; //+ 1 for bias

		outputs[i] = new double[n_layer_neurons];
		errors[i] = new double[n_layer_neurons];
		deltas[i] = new double*[n_layer_neurons];
		weights[i] = new double*[n_layer_neurons];

		for (unsigned int j = 0; j < n_layer_neurons; ++j) {
			outputs[i][j] = 0;
			errors[i][j] = 0;
			deltas[i][j] = new double[n_neuron_inputs];
			weights[i][j] = new double[n_neuron_inputs];

			for (unsigned int k = 0; k < n_neuron_inputs; ++k) {
				deltas[i][j][k] = 0;
				weights[i][j][k] = dmath::random(); // TODO : Refactor
			}
		}


	}
}
Backprop::~Backprop() {
	const unsigned int n_layers_ns = n_layers - 1; // n_layers non simulated, ie without input layer semantics

	for (unsigned int i = 0;i < n_layers_ns; ++i) {
		const unsigned int i_s = i + 1; // i with input layer semantics (for config use only)
		const unsigned int n_layer_neurons = config[i_s];

		for (unsigned int j = 0; j < n_layer_neurons; ++j) {
			delete[] deltas[i][j];
			delete[] weights[i][j];
		}

		delete[] outputs[i];
		delete[] errors[i];
		delete[] deltas[i];
		delete[] weights[i];
	}
	delete[] outputs;
	delete[] errors;
	delete[] deltas;
	delete[] weights;

	delete[] config;
}

const double* Backprop::activate(const double input[]) {
	const unsigned int n_layers_ns = n_layers - 1;
	const unsigned int output_layer = n_layers_ns - 1;

	for (unsigned int i = 0;i < n_layers_ns; ++i) { // foreach layer
		const unsigned int i_s = i + 1; // i with input layer semantics (for config use only)

		const unsigned int lower_layer = i - 1;
		const unsigned int lower_layer_s = i_s - 1;

		const unsigned int n_layer_inputs = config[lower_layer_s];
		const unsigned int n_layer_neurons = config[i_s];

		for (unsigned int j = 0;j < n_layer_neurons; ++j) { // foreach neuron
			//neuron_outputs[i][j] = input[k] * weights[i][j][k];
			double sum = 0;
			for (unsigned int k = 0;k < n_layer_inputs; ++k) { // foreach weight
				const double input_k = i > 0 ? outputs[lower_layer][k] : input[k]; // input layer semantics

				sum += input_k * weights[i][j][k];
			}
			sum += bias * weights[i][j][n_layer_inputs];
			outputs[i][j] = activation(sum);
		}
	}
	return outputs[output_layer];
}

void Backprop::train(const double input[], const double desired[]) {
	activate(input);

	// calculated constants
	const double inverse_momentum = 1.0 - momentum;
	const unsigned int n_layers_ns = n_layers - 1;

	const unsigned int output_layer = n_layers_ns - 1;
	const unsigned int output_layer_s = n_layers - 1; ; // output_layer with input layer semantics (for config use only)

	const unsigned int n_outputs = config[output_layer_s];
	const unsigned int n_hidden_layers = output_layer - 1;

	// calculate error for output layer
	for (unsigned int j = 0; j < n_outputs; ++j) {
		const unsigned int i = output_layer;
		const double output_layer_input = i > 0 ? outputs[i][j] : input[j]; // input layer semantics

		//errors[output_layer][j] = f'(outputs[i][j]) * (desired[j] - outputs[i][j]);
		errors[output_layer][j] = gradient(output_layer_input) * (desired[j] - output_layer_input);
	}

	// calculate error for hidden layers
	for (unsigned int i = n_hidden_layers; i <= n_layers_ns; --i) { // i <= n_layers_ns, unsigned numeric overflow
		const unsigned int i_s = i + 1; // i with input layer semantics (for config use only)
		const unsigned int upper_layer = i + 1;
		const unsigned int upper_layer_s = i_s + 1;

		const unsigned int n_layer_neurons = config[i_s];
		const unsigned int n_upperlayer_neurons = config[upper_layer_s];

		for (unsigned int j = 0; j < n_layer_neurons; ++j) {
			//error[i][j] = f'(upperlayer_output[i+1][j]) * SUMOF([upperlayer_error[i+1][k] * upperlayer_weights[i+1][k][j])
			double sum = 0;
			for (unsigned int k = 0; k < n_upperlayer_neurons; ++k) {
				sum += errors[upper_layer][k] * weights[upper_layer][k][j];
			}

			errors[i][j] = gradient(outputs[i][j]) * sum;
		}
	}

	// adjust weights for layers
	for (unsigned int i = 0; i < n_layers_ns; ++i) {
		const unsigned int i_s = i + 1; // i with input layer semantics (for config use only)
		const unsigned int lower_layer = i - 1;
		const unsigned int lower_layer_s = i_s - 1;

		const unsigned int n_layer_inputs = config[lower_layer_s];
		const unsigned int n_layer_neurons = config[i_s];

		for (unsigned int j = 0; j < n_layer_neurons; ++j) {
			for (unsigned int k = 0; k < n_layer_inputs; ++k) {
				// deltas[i][j][k] = (1 - momentum) * (learn_rate * errors[i][j]) * lowerlayer_outputs[i-1][k] + (momentum * deltas[i][j][k])
				const double lower_layer_output = i > 0 ? outputs[lower_layer][k] : input[k]; // input layer semantics
				const double delta = inverse_momentum * (learn_rate * errors[i][j]) * lower_layer_output + (momentum * deltas[i][j][k]);

				deltas[i][j][k] = delta;
				weights[i][j][k] += delta;
			}
			// bias
			const double delta = inverse_momentum * (learn_rate * errors[i][j]) * bias + (momentum * deltas[i][j][n_layer_inputs]);

			deltas[i][j][n_layer_inputs] = delta;
			weights[i][j][n_layer_inputs] += delta;
		}
	}
}
