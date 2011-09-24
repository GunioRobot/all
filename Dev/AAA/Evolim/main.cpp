#include "neural_network.hpp"
#include <iostream>
#include <cstdlib>

int main() {
	std::srand(1000);
	Neural::Network test(70, 1);

	for (int z = 0; z < 50000; ++z) {
		std::vector< std::vector<Neural::Dendrite> > adjacencylist;
		{
			adjacencylist.clear();
			adjacencylist.resize(70);
			for (size_t i = 0; i < adjacencylist.size(); ++i) {
				for (size_t j = 0; j < (adjacencylist.size() / 10); ++j) {
					bool inhibitory = (std::rand() % 4) > 2 ? 1 : 0;
					size_t parent = (std::rand() % adjacencylist.size());

					//std::cout << parent << ' ' << inhibitory << '\t';

					adjacencylist[i].push_back( Neural::Dendrite( parent, inhibitory ) );
				}
				//std::cout << '\n';
			}
			test.load_adjacency_list(adjacencylist);
		}


		for (size_t y = 0; y < 4; ++y) {
			test.fire(1);
			test.fire(2);
			test.run();

			for(size_t j = 0; j < adjacencylist.size(); ++j) {
				if (test.hasfired(j)) {
					//std::cout << 1;
				} else {
					//std::cout << 0;
				}
			}
			//std::cout << '\n';
		}
	}
	return 0;
}
