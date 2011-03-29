#include <iostream>

#include <boost/array.hpp>
#include <nac.hpp>
#include <nac_human.hpp>

class AI : public nac::Player {
	char opponent;
public:
	AI(unsigned char id, unsigned char opponent) : Player(id), opponent(opponent) {}
	void Turn(nac::Board& board) {
		const unsigned char* markings = board.getBoard();

		for (int i = 0; i < 9; ++i) {
			std::cout << markings[i] << '\n';
		}
	}
	void Win() {}
	void Loss() {}
};

boost::array<int, 3> buildArray(int n) {
	boost::array<int, 3> ar;
	for (int i = 0; i < 3; ++i) ar[i] = n;

	std::cout << &ar << '\n';

	return ar;
}

int main() {
	boost::array<int,3> myarray = buildArray(5);
	std::cout << &myarray << '\n';

	int ar[3];
	std::cout << &ar << '\n';

	boost::array<int, 5> vector5i;

	for (int i = 0; i < 5; ++i) {
		vector5i[i] = i;
		std::cout << vector5i[i];
	}


	return 0;
}
