#include "check_board.h"

Check_Board::Check_Board()
{

}

Check_Board::~Check_Board()
{

}

// 確認済か
bool Check_Board::isConfirmed(const int no) const
{
	switch (getBoardElement(no))
	{
	case Combined:		return true;	break;
	case Uncombined:	return true;	break;
	default:			return false;	break;
	}
	return false;
}

// チェック中か
bool Check_Board::isChecking(const int no) const
{
	if (getBoardElement(no) == Checking)	return true;
	return false;
}

// 結合済か
bool Check_Board::isCombined(const int no) const
{
	if (getBoardElement(no) == Combined)	return true;
	return false;
}

// ある要素をチェック状態にする
void Check_Board::setBoardElementChecking(const int no)
{
	setBoardElement(no, Checking);
}

// ある要素を結合状態にする
void Check_Board::setBoardElementCombined(const int no)
{
	setBoardElement(no, Combined);
}

// ある要素を未結合状態にする
void Check_Board::setBoardElementUncombined(const int no)
{
	setBoardElement(no, Uncombined);
}

// 上がelementと同じ状態か
bool Check_Board::checkUpper(const int no, const int element) const
{
	if (getBoardElement(no - column_size) == element)	return true;
	return false;
}

// 右がelementと同じ状態か
bool Check_Board::checkRight(const int no, const int element) const
{
	if (getBoardElement(no + 1) == element)	return true;
	return false;
}

// 下がelementと同じ状態か
bool Check_Board::checkLower(const int no, const int element) const
{
	if (getBoardElement(no + column_size) == element)	return true;
	return false;
}

// 左がelementと同じ状態か
bool Check_Board::checkLeft(const int no, const int element) const
{
	if (getBoardElement(no - 1) == element)	return true;
	return false;
}