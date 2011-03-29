#ifndef _NAC_HUMAN_HPP_
	#define _NAC_HUMAN_HPP_

#include <iostream>

class Human : public nac::Player {
public:
	Human(unsigned char id) : Player(id) {}
	void Turn(nac::Board& board) {
		unsigned int x = 0, y = 0;

		std::cout << "Player (" << getID() << ") Enter Y,X: ";
		std::cin >> x >> y;
		std::cout << std::endl;

		while (!board.Mark(getID(), x, y)) {
			if (x > 2 || y > 2) std::cout << "Valid Y, X values are between 0 and 2;\n";
			std::cout << "--(" << x << ")(" << y << ") Invalid\n";
			std::cout << "Player (" << getID() << ") Enter Y,X: ";
			std::cin >> x >> y;
		}
	}
	void Win() { std::cout << "Player " << getID() << " won!\n"; }
	void Loss() { std::cout << "Player " << getID() << " lost!\n"; }
};

#endif
