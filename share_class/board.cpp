#include "board.h"

Board::Board()
{
	board = board_entity;
}

Board::~Board()
{

}

int* Board::getBoard() const
{
	return board;
}

void Board::setBoard(const int* input_board)
{
	int i;
	for (i = 0; i < board_size; ++i)
	{
		board[i] = input_board[i];
	}
}

int Board::getBoardElement(const int no) const
{
	return board[no];
}

void Board::setBoardElement(const int no, const int val)
{
	board[no] = val;
}

void Board::print() const
{
	int i;
	for (i = 0; i < row_size; ++i) {
		std::cout 
			<< getBoardElement(i * column_size + 0) 
			<< getBoardElement(i * column_size + 1)
			<< getBoardElement(i * column_size + 2)
			<< getBoardElement(i * column_size + 3)
			<< getBoardElement(i * column_size + 4)
			<< getBoardElement(i * column_size + 5)
			<< getBoardElement(i * column_size + 6)
			<< getBoardElement(i * column_size + 7)
			<< std::endl;
	}
}

void Board::print(const std::string title) const
{
	std::cout << title << std::endl;
	Board::print();
}