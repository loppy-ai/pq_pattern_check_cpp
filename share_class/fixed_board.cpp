#include "fixed_board.h"

Fixed_Board::Fixed_Board(const int board_pattern)
{
	setFixedBoard(board_pattern);
}

Fixed_Board::~Fixed_Board()
{

}

void Fixed_Board::setFixedBoard(const int board_pattern)
{
	switch (board_pattern) {
	case 1: setBoard(board_pattern_1);	break;
	case 2:	setBoard(board_pattern_2);	break;
	case 3:	setBoard(board_pattern_3);	break;
	case 4:	setBoard(board_pattern_4);	break;
	case 5:	setBoard(board_pattern_5);	break;
	case 6:	setBoard(board_pattern_6);	break;
	case 7:	setBoard(board_pattern_7);	break;
	case 8:	setBoard(board_pattern_8);	break;
	case 101:	setBoard(board_pattern_101);	break;
	case 102:	setBoard(board_pattern_102);	break;
	case 103:	setBoard(board_pattern_103);	break;
	case 104:	setBoard(board_pattern_104);	break;
	case 105:	setBoard(board_pattern_105);	break;
	case 106:	setBoard(board_pattern_106);	break;
	case 107:	setBoard(board_pattern_107);	break;
	case 108:	setBoard(board_pattern_108);	break;
	case 109:	setBoard(board_pattern_109);	break;
	case 110:	setBoard(board_pattern_110);	break;
	case 111:	setBoard(board_pattern_111);	break;
	case 112:	setBoard(board_pattern_112);	break;
	case 113:	setBoard(board_pattern_113);	break;
	case 114:	setBoard(board_pattern_114);	break;
	case 115:	setBoard(board_pattern_115);	break;
	case 116:	setBoard(board_pattern_116);	break;
	default:
		std::cout << "Error : board_pattern‚Ìİ’è‚ÉŒë‚è‚ª‚ ‚è‚Ü‚·" << std::endl;
		std::cout << std::endl;
		exit(1);
		break;
	}
}