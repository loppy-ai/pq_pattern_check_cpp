#include "check_board.h"

Check_Board::Check_Board()
{

}

Check_Board::~Check_Board()
{

}

bool Check_Board::isConfirmed(const int no) const
{
	bool confirmed_flag;
	switch (getBoardElement(no))
	{
	case Combined:
		confirmed_flag = true;
		break;
	case Uncombined:
		confirmed_flag = true;
		break;
	default:
		confirmed_flag = false;
		break;
	}
	return confirmed_flag;
}

bool Check_Board::isChecking(const int no) const
{
	bool checking_flag;
	if (getBoardElement(no) == Checking) {
		checking_flag = true;
	}
	else {
		checking_flag = false;
	}
	return checking_flag;
}

bool Check_Board::isCombined(const int no) const
{
	bool combined_flag;
	if (getBoardElement(no) == Combined) {
		combined_flag = true;
	}
	else {
		combined_flag = false;
	}
	return combined_flag;
}

void Check_Board::setBoardElementChecking(const int no)
{
	setBoardElement(no, Checking);
}

void Check_Board::setBoardElementCombined(const int no)
{
	setBoardElement(no, Combined);
}

void Check_Board::setBoardElementUncombined(const int no)
{
	setBoardElement(no, Uncombined);
}

bool Check_Board::checkUpper(const int no, const int element) const
{
	bool match_flag;
	if (getBoardElement(no - column_size) == element) {
		match_flag = true;
	}
	else {
		match_flag = false;
	}
	return match_flag;
}

bool Check_Board::checkRight(const int no, const int element) const
{
	bool match_flag;
	if (getBoardElement(no + 1) == element) {
		match_flag = true;
	}
	else {
		match_flag = false;
	}
	return match_flag;
}

bool Check_Board::checkLower(const int no, const int element) const
{
	bool match_flag;
	if (getBoardElement(no + column_size) == element) {
		match_flag = true;
	}
	else {
		match_flag = false;
	}
	return match_flag;
}

bool Check_Board::checkLeft(const int no, const int element) const
{
	bool match_flag;
	if (getBoardElement(no - 1) == element) {
		match_flag = true;
	}
	else {
		match_flag = false;
	}
	return match_flag;
}