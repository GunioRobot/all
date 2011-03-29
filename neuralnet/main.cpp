#include <dmath.h>
#include <backprop.h>
#include <iostream>

void Backprop::print() const {
	std::cout << "\nLayers : " << n_layers;
	std::cout << "\nConfig : ";
	for (unsigned int i = 0;i < n_layers; ++i) {
		std::cout << config[i] << ", ";
	}
	for (unsigned int i = 1;i < n_layers; ++i) {
		const unsigned int i_ns = i - 1; //offset for weights, deltas, errors (whom exclude an input layer)
		std::cout << "\nLayer[" << i << "] (" << i_ns << ')';
		std::cout << "\n	Outputs[i] = " << outputs[i_ns];
		std::cout << "\n	Errors[i] = " << errors[i_ns];
		std::cout << "\n	Deltas[i] = " << deltas[i_ns];
		std::cout << "\n	Weights[i] = " << weights[i_ns];
		std::cout << '\n';

		for (unsigned int j = 0; j < config[i]; ++j) {
			std::cout << "\n	Neuron[" << i << "][" << j << ']';
			std::cout << "\n		Outputs[i][j] = " << outputs[i_ns][j];
			std::cout << "\n		Errors[i][j] = " << errors[i_ns][j];
			std::cout << "\n		Deltas[i][j] = " << deltas[i_ns][j];
			std::cout << "\n		Weights[i][j] = " << weights[i_ns][j];
			std::cout << '\n';

			for (unsigned int k = 0; k < config[i_ns] + 1; ++k) {
				std::cout << "\n		Weight[" << i << "][" << j << "][" << k << ']';
				if (k == config[i - 1]) {
					std::cout << " (BIAS)";
				}
				std::cout << "\n			Deltas[i][j][k] = " << deltas[i_ns][j][k];
				std::cout << "\n			Weights[i][j][k] = " << weights[i_ns][j][k];
				std::cout << '\n';
			}
		}
	}
}

int main() {
	//dmath::randomseed();

	double XOR[4][2];
	double dXOR[4][1];

	XOR[0][0] = 1; XOR[0][1] = 1;
	XOR[1][0] = 1; XOR[1][1] = 0;
	XOR[2][0] = 0; XOR[2][1] = 1;
	XOR[3][0] = 0; XOR[3][1] = 0;

	dXOR[0][0] = -0.5;
	dXOR[1][0] = 0.5;
	dXOR[2][0] = 0.5;
	dXOR[3][0] = -0.5;

	const int n_layers = 3;
	unsigned int config[n_layers] = {2, 20, 1};
	double bias = 1, learn_rate = 0.2 * (4.0/40.0), momentum = 0.5 * (4.0/40.0);

	Backprop ann(config, n_layers, bias, learn_rate, momentum);
	for (int i = 0; i < 200; ++i) for (int j = 0; j < 4; ++j) ann.train(XOR[j], dXOR[j]);

	ann.print();

	for (int j = 0; j < 4; ++j) {
		const double* output = ann.activate(XOR[j]);

		std::cout << "\nXOR(" << j << ") Results ";
		for (unsigned int k = 0; k < 1; ++k) std::cout << output[k] << ',';
	}

	return 0;
}

