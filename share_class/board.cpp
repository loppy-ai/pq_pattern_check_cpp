#include "board.h"

Board::Board()
{
	board = board_entity;
}

Board::~Board()
{

}

// �Ֆʂ̎擾
int* Board::getBoard()
{
	return board;
}

// �Ֆʂ̐ݒ�
void Board::setBoard(const int* input_board)
{
	int i;
	for (i = 0; i < board_size; ++i) {
		setBoardElement(i, input_board[i]);
	}
}

// �Ֆʂ̗v�f���擾
int Board::getBoardElement(const int no) const
{
	return board[no];
}

// �Ֆʂɗv�f��ݒ�
void Board::setBoardElement(const int no, const int val)
{
	board[no] = val;
}

// �F�Ղ悩
bool Board::isColorPuyo(const int no) const
{
	bool color_puyo_flag;
	switch (getBoardElement(no))
	{
	case Ojama:		// ������܂Ղ�
		color_puyo_flag = false;
		break;
	case Kata:		// �����Ղ�
		color_puyo_flag = false;
		break;
	case Heart:		// �n�[�g
		color_puyo_flag = false;
		break;
	case None:		// ����
		color_puyo_flag = false;
		break;
	case Prism:		// �v���Y��
		color_puyo_flag = false;
		break;
	default:
		color_puyo_flag = true;
		break;
	}
	return color_puyo_flag;
}

// ���͂Ɋ������܂�ď����邩
bool Board::isInfluenced(const int no) const
{
	bool influenced_flag;
	switch (getBoardElement(no))
	{
	case Ojama:		// ������܂Ղ�
		influenced_flag = true;
		break;
	case Kata:		// �����Ղ�
		influenced_flag = true;
		break;
	case Heart:		// �n�[�g
		influenced_flag = true;
		break;
	case Prism:		// �v���Y��
		influenced_flag = true;
		break;
	default:
		influenced_flag = false;
		break;
	}
	return influenced_flag;
}

// �Ղ悪���݂��Ȃ���
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

// �Ղ�̌������N��������
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

// ��i�����邩
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

// �E�񂪂��邩
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

// ���i�����邩
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

// ���񂪂��邩
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

// ���݂̉ӏ�����̐F�Ɠ�����
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

// ���݂̉ӏ����E�̐F�Ɠ�����
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

// ���݂̉ӏ������̐F�Ɠ�����
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

// ���݂̉ӏ������̐F�Ɠ�����
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

// �Ֆʕ\��
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

// �Ֆʕ\��
void Board::print(const std::string title) const
{
	std::cout << title << std::endl;
	Board::print();
	std::cout << std::endl;
}