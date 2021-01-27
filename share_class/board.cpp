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
	int i;
	for (i = 0; i < board_size; ++i) {
		setBoardElement(i, input_board[i]);
	}
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
	bool color_puyo_flag;
	switch (getBoardElement(no))
	{
	case Ojama:		// おじゃまぷよ
		color_puyo_flag = false;
		break;
	case Kata:		// かたぷよ
		color_puyo_flag = false;
		break;
	case Heart:		// ハート
		color_puyo_flag = false;
		break;
	case None:		// 無し
		color_puyo_flag = false;
		break;
	case Prism:		// プリズム
		color_puyo_flag = false;
		break;
	default:
		color_puyo_flag = true;
		break;
	}
	return color_puyo_flag;
}

// 周囲に巻き込まれて消えるか
bool Board::isInfluenced(const int no) const
{
	bool influenced_flag;
	switch (getBoardElement(no))
	{
	case Ojama:		// おじゃまぷよ
		influenced_flag = true;
		break;
	case Kata:		// かたぷよ
		influenced_flag = true;
		break;
	case Heart:		// ハート
		influenced_flag = true;
		break;
	case Prism:		// プリズム
		influenced_flag = true;
		break;
	default:
		influenced_flag = false;
		break;
	}
	return influenced_flag;
}

// ぷよが存在しないか
bool Board::isNone(const int no) const
{
	bool none_flag;
	if (getBoardElement(no) == None) {
		none_flag = true;
	}
	else {
		none_flag = false;
	}
	return none_flag;
}

// ぷよの結合が起こったか
bool Board::isEliminationHappened() const
{
	int i;
	bool elimination_flag = false;
	for (i = 0; i < board_size; ++i) {
		if (getBoardElement(i) == Elimination) {
			elimination_flag = true;
			break;
		}
	}
	return elimination_flag;
}

// 上段があるか
bool Board::canGetUpperRow(const int no) const
{
	bool can_get_flag;
	if (no >= column_size) {
		can_get_flag = true;
	}
	else {
		can_get_flag = false;
	}
	return can_get_flag;
}

// 右列があるか
bool Board::canGetRightColumn(const int no) const
{
	bool can_get_flag;
	if (no % column_size != column_size - 1) {
		can_get_flag = true;
	}
	else {
		can_get_flag = false;
	}
	return can_get_flag;
}

// 下段があるか
bool Board::canGetLowerRow(const int no) const
{
	bool can_get_flag;
	if (no < board_size - column_size) {
		can_get_flag = true;
	}
	else {
		can_get_flag = false;
	}
	return can_get_flag;
}

// 左列があるか
bool Board::canGetLeftColumn(const int no) const
{
	bool can_get_flag;
	if (no % column_size != 0) {
		can_get_flag = true;
	}
	else {
		can_get_flag = false;
	}
	return can_get_flag;
}

// 現在の箇所が上の色と同じか
bool Board::isSameUpper(const int no) const
{
	bool is_same_flag;
	if (getBoardElement(no) == getBoardElement(no - column_size)) {
		is_same_flag = true;
	}
	else {
		is_same_flag = false;
	}
	return is_same_flag;
}

// 現在の箇所が右の色と同じか
bool Board::isSameRight(const int no) const
{
	bool is_same_flag;
	if (getBoardElement(no) == getBoardElement(no + 1)) {
		is_same_flag = true;
	}
	else {
		is_same_flag = false;
	}
	return is_same_flag;
}

// 現在の箇所が下の色と同じか
bool Board::isSameLower(const int no) const
{
	bool is_same_flag;
	if (getBoardElement(no) == getBoardElement(no + column_size)) {
		is_same_flag = true;
	}
	else {
		is_same_flag = false;
	}
	return is_same_flag;
}

// 現在の箇所が左の色と同じか
bool Board::isSameLeft(const int no) const
{
	bool is_same_flag;
	if (getBoardElement(no) == getBoardElement(no - 1)) {
		is_same_flag = true;
	}
	else {
		is_same_flag = false;
	}
	return is_same_flag;
}

// 盤面表示
void Board::print() const
{
	int i, j;
	std::string str_board[column_size];
	for (i = 0; i < row_size; ++i) {
		for (j = 0; j < column_size; ++j) {
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