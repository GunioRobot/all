#ifndef _NAC_HPP_
	#define _NAC_HPP_

#include <iostream>
#include <cassert>

namespace nac {
	class Board {
		unsigned char board[3][3];
		unsigned char nil; // > 0
		unsigned char lastplayer; // Single turns

	public:
		Board(unsigned char nil = ' ') : nil(nil) {
			for (int i = 0; i < 3; ++i) {
			for (int j = 0; j < 3; ++j) {
				board[i][j] = nil;
			}}
			lastplayer = nil;
		}
		const unsigned char* getBoard() const { return &board[0][0]; }
		unsigned char getNil() const { return nil; }

		bool Mark(unsigned char player, unsigned int x, unsigned int y) {
			assert(lastplayer != player);
			if (x > 2 || y > 2) return false;
			unsigned char& selected = board[x][y];
			if (selected != nil) return false;
			selected = player; lastplayer = player;
			return true;
		}
		bool hasWon(unsigned char player) const {
			for (unsigned int i = 0; i < 3; ++i) {
				if ((board[i][0] == player) && (board[i][1] == player) && (board[i][2] == player)) return true;
				if ((board[0][i] == player) && (board[1][i] == player) && (board[2][i] == player)) return true;
			}
			if ((board[0][0] == player) && (board[1][1] == player) && (board[2][2] == player)) return true;
			if ((board[0][2] == player) && (board[1][1] == player) && (board[2][0] == player)) return true;
			return false;
		}
		void print() const {
			for (int i = 0; i < 3; ++i) {
				std::cout << i << ' ';
				for (int j = 0; j < 3; ++j) {
					std::cout << '|' << board[i][j];
				}
				std::cout << "|" << std::endl;
			}
			std::cout << "   0 1 2" << std::endl;
		}
	};

	class Player {
		unsigned char id;
	public:
		Player(unsigned char id) : id(id) {}
		char getID() const { return id; }

		virtual void Turn(Board& board)=0;
		virtual void Win()=0;
		virtual void Loss()=0;
	};

	class Game {
		Player *p1, *p2;
	public:
		Game(Player* p1, Player* p2) : p1(p1), p2(p2) {}
		void Run() {
			Board board;
			board.print();

			int framesleft = 9;
			while (--framesleft > 0) {
				p1->Turn(board);
				board.print();
				if (board.hasWon(p1->getID())) {p1->Win(); p2->Loss(); break;}

				p2->Turn(board);
				board.print();
				if (board.hasWon(p2->getID())) {p2->Win(); p1->Loss(); break;}
			}
		}
	};
}

#endif
