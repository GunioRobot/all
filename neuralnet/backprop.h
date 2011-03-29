class Backprop {
public:
	double bias;
	double learn_rate;
	double momentum;

	double **errors;
	double **outputs;
	double ***deltas;
	double ***weights;

	unsigned int *config;
	unsigned int n_layers;

	Backprop(const unsigned int config[], const unsigned int n_layers, const double bias, const double learn_rate, const double momentum);
	~Backprop();

	const double* activate(const double input[]);
	void train(const double input[], const double desired[]);
	void print() const;
}; //stack size == 72 bits (9 bytes)
