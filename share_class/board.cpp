#include "board.h"

Board::Board()
{
	board = board_entity;
}

Board::~Board()
{

}

// 盤面の取得
int* Board::getBoard()
{
	return board;
}

// 盤面の設定
void Board::setBoard(const int* input_board)
{
	std::memcpy(board, input_board, board_size * 4);	// int:4byte
}

// 盤面の要素を取得
int Board::getBoardElement(const int no) const
{
	return board[no];
}

// 盤面に要素を設定
void Board::setBoardElement(const int no, const int val)
{
	board[no] = val;
}

// 色ぷよか
bool Board::isColorPuyo(const int no) const
{
	switch (getBoardElement(no))
	{
	case Ojama:	return false;	break;
	case Kata:	return false;	break;
	case Heart:	return false;	break;
	case None:	return false;	break;
	case Prism:	return false;	break;
	default:	return true;
	}
	return true;
}

// 周囲に巻き込まれて消えるか
bool Board::isInfluenced(const int no) const
{
	switch (getBoardElement(no))
	{
	case Ojama:	return true;	break;
	case Kata:	return true;	break;
	case Heart:	return true;	break;
	case Prism:	return true;	break;
	default:	return false;	break;
	}
	return false;
}

// ぷよが存在しないか
bool Board::isNone(const int no) const
{
	if (getBoardElement(no) == None)	return true;
	return false;
}

// ぷよの結合が起こったか
bool Board::isEliminationHappened() const
{
	for (int i = 0; i < board_size; ++i) {
		if (getBoardElement(i) == Elimination)	return true;
	}
	return false;
}

// 上段があるか
bool Board::canGetUpperRow(const int no) const
{
	if (no >= column_size)	return true;
	return false;
}

// 右列があるか
bool Board::canGetRightColumn(const int no) const
{
	if (no % column_size != column_size - 1)	return true;
	return false;
}

// 下段があるか
bool Board::canGetLowerRow(const int no) const
{
	if (no < board_size - column_size)	return true;
	return false;
}

// 左列があるか
bool Board::canGetLeftColumn(const int no) const
{
	if (no % column_size != 0)	return true;
	return false;
}

// 現在の箇所が上の色と同じか
bool Board::isSameUpper(const int no) const
{
	if (getBoardElement(no) == getBoardElement(no - column_size))	return true;
	return false;
}

// 現在の箇所が右の色と同じか
bool Board::isSameRight(const int no) const
{
	if (getBoardElement(no) == getBoardElement(no + 1))	return true;
	return false;
}

// 現在の箇所が下の色と同じか
bool Board::isSameLower(const int no) const
{
	if (getBoardElement(no) == getBoardElement(no + column_size))	return true;
	return false;
}

// 現在の箇所が左の色と同じか
bool Board::isSameLeft(const int no) const
{
	if (getBoardElement(no) == getBoardElement(no - 1))	return true;
	return false;
}

// 盤面表示
void Board::print() const
{
	std::string str_board[column_size];
	for (int i = 0; i < row_size; ++i) {
		for (int j = 0; j < column_size; ++j) {
			if (getBoardElement(i * column_size + j) == None) {
				str_board[j] = ".";
			}
			else {
				str_board[j] = std::to_string(getBoardElement(i * column_size + j));
			}
		}
		std::cout 
			<< str_board[0] << " "
			<< str_board[1] << " "
			<< str_board[2] << " "
			<< str_board[3] << " "
			<< str_board[4] << " "
			<< str_board[5] << " "
			<< str_board[6] << " "
			<< str_board[7] << " "
			<< std::endl;
	}
}

// 盤面表示
void Board::print(const std::string title) const
{
	std::cout << title << std::endl;
	Board::print();
	std::cout << std::endl;
}