#include "fixed_board.h"

Fixed_Board::Fixed_Board(const int board_pattern)
{
	setFixedBoard(board_pattern);
}

Fixed_Board::~Fixed_Board()
{

}

// 指定盤面をセット
void Fixed_Board::setFixedBoard(const int board_pattern)
{
	switch (board_pattern) {
	case 1:		setBoard(board_pattern_1);		break;
	case 2:		setBoard(board_pattern_2);		break;
	case 3:		setBoard(board_pattern_3);		break;
	case 4:		setBoard(board_pattern_4);		break;
	case 5:		setBoard(board_pattern_5);		break;
	case 6:		setBoard(board_pattern_6);		break;
	case 7:		setBoard(board_pattern_7);		break;
	case 8:		setBoard(board_pattern_8);		break;
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
	case 201:	setBoard(board_pattern_201);	break;
	case 202:	setBoard(board_pattern_202);	break;
	case 203:	setBoard(board_pattern_203);	break;
	case 204:	setBoard(board_pattern_204);	break;
	case 205:	setBoard(board_pattern_205);	break;
	case 206:	setBoard(board_pattern_206);	break;
	case 207:	setBoard(board_pattern_207);	break;
	case 208:	setBoard(board_pattern_208);	break;
	case 211:	setBoard(board_pattern_211);	break;
	case 212:	setBoard(board_pattern_212);	break;
	case 213:	setBoard(board_pattern_213);	break;
	case 214:	setBoard(board_pattern_214);	break;
	case 215:	setBoard(board_pattern_215);	break;
	case 216:	setBoard(board_pattern_216);	break;
	case 217:	setBoard(board_pattern_217);	break;
	case 218:	setBoard(board_pattern_218);	break;
	case 301:	setBoard(board_pattern_301);	break;
	case 302:	setBoard(board_pattern_302);	break;
	case 303:	setBoard(board_pattern_303);	break;
	case 304:	setBoard(board_pattern_304);	break;
	case 305:	setBoard(board_pattern_305);	break;
	case 306:	setBoard(board_pattern_306);	break;
	case 307:	setBoard(board_pattern_307);	break;
	case 308:	setBoard(board_pattern_308);	break;
	case 311:	setBoard(board_pattern_311);	break;
	case 312:	setBoard(board_pattern_312);	break;
	case 313:	setBoard(board_pattern_313);	break;
	case 314:	setBoard(board_pattern_314);	break;
	case 315:	setBoard(board_pattern_315);	break;
	case 316:	setBoard(board_pattern_316);	break;
	case 317:	setBoard(board_pattern_317);	break;
	case 318:	setBoard(board_pattern_318);	break;
	case 323:   setBoard(board_pattern_323);    break;
	default:
		std::cout << "Error : board_patternの設定に誤りがあります" << std::endl;
		std::cout << std::endl;
		exit(1);
		break;
	}
}