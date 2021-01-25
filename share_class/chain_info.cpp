#include "chain_info.h"

Chain_Info::Chain_Info(const Param_Info *pi, Next *next, Board *board, const Board *trace_pattern_board)
{
	chain(pi, next, board, trace_pattern_board);
}

Chain_Info::~Chain_Info()
{

}

void Chain_Info::chain(const Param_Info *pi, Next *next, Board *board, const Board *trace_pattern_board)
{
	bool chaining_flag = true;
	int chain_count;
	if (pi->isProcessPrint()) {
		std::cout << "---------------------連鎖過程---------------------" << std::endl;
	}
	debugPrint(pi, next, board, "default");
	debugPrint(pi, trace_pattern_board, "tracePattern");
	// 盤面に対してなぞり消しパターンを適用
	applyTracePattern(board, trace_pattern_board);
	debugPrint(pi, next, board, "applyedTracePattern");
	// 消えた部分を落とす
	dropBoard(board);
	debugPrint(pi, next, board, "dropBoard");

	// 連鎖数分ループ
	for (chain_count = 0; chain_count <= max_num_of_chain; /* no-increment */ ) {
		// 結合チェック用盤面インスタンスの生成
		Check_Board check_board;
		// 結合チェック
		checkConnection(chain_count, pi, board, &check_board);
		// 結合チェック用盤面インスタンスの破棄
		// delete &check_board;
		// ぷよが消えた場合
		if (board->isEliminationHappened()) {
			// 次の連鎖
			++chain_count;
			debugPrint(pi, next, board, "elimination");
			debugChain(chain_count);
			dropBoard(board);
			debugPrint(pi, next, board, "dropBoard");
		}
		// 消えなかった場合
		else {
			// nextを落とす
			bool drop_next_flag;
			drop_next_flag = dropNext(next, board);
			debugPrint(pi, next, board, "dropNext");
			if (!drop_next_flag) {
				chaining_flag = false;
			}
		}
		if (chaining_flag == false) {
			num_of_chain = chain_count;
			break;
		}
	}
}

void Chain_Info::applyTracePattern(Board *board, const Board *trace_pattern_board) {
	int i;
	for (i = 0; i < board_size; ++i) {
		if (trace_pattern_board->getBoardElement(i) == 1) {
			board->setBoardElement(i, Elimination);
		}
	}
}

void Chain_Info::dropBoard(Board *board)
{
	// 0:なぞったor結合して消えた 9:落ちて消えた
	int i, target;
	// 盤面走査
	for (i = 0; i < board_size; ++i) {
		// 0以外は次を見る
		if (board->getBoardElement(end_of_board - i) != Elimination) {
			continue;
		}
		// 0だったら1つ上を見る
		for (target = end_of_board - i - column_size; target >= 0; target -= column_size) {
			if (board->getBoardElement(target) != Elimination) {
				// 1つ上が0以外だったら落として、1つ上を0にする
				board->setBoardElement(end_of_board - i, board->getBoardElement(target));
				board->setBoardElement(target, Elimination);
				break;
			}
		}
	}
	// 落ちて消えたところを9にする
	for (i = 0; i < board_size; ++i) {
		if (board->getBoardElement(i) == 0) {
			board->setBoardElement(i, None);
		}
	}
}

void Chain_Info::checkConnection(const int chain_count, const Param_Info *pi, Board *board, Check_Board *check_board)
{
	int i, j;
	// 色ぷよ以外はチェック済(未結合)にする
	for (i = 0; i < board_size; ++i) {
		if (!board->isColorPuyo(i)) {
			check_board->setBoardElementUncombined(i);
		}
	}
	// 結合チェック
	for (i = 0; i < board_size; ++i) {
		// 確定していたら飛ばす
		if (check_board->isConfirmed(i)) {
			continue;
		}
		// 結合数
		int count = 0;
		// 再帰結合チェック
		count = recursionCheckConnection(i, board, check_board, count);
		// i番目のぷよは何個繋がっているかが返ってくる
		// 消える数だけ繋がっている場合
		if (count >= pi->getMaxConnection()) {
			// all_chain_infoに情報格納
			setElementCount(board->getBoardElement(i), chain_count, count);
			setSeparateCount(board->getBoardElement(i), chain_count);
			for (j = 0; j < board_size; ++j) {
				if (check_board->isChecking(j)) {
					check_board->setBoardElementCombined(j);
				}
			}
		}
		// 消える数だけ繋がっていなかった場合
		else {
			for (j = 0; j < board_size; ++j) {
				if (check_board->isChecking(j)) {
					check_board->setBoardElementUncombined(j);
				}
			}
		}
	}
	// 結合していたところの盤面を0にする
	for (i = 0; i < board_size; ++i) {
		if (check_board->isCombined(i)) {
			board->setBoardElement(i, Elimination);
		}
	}
	// 周囲の影響を受けるものの処理
	for (i = 0; i < board_size; ++i) {
		if (board->isInfluenced(i)) {
			if ((board->canGetUpperRow(i) && check_board->checkUpper(i, Combined))
				|| (board->canGetRightColumn(i) && check_board->checkRight(i, Combined))
				|| (board->canGetLowerRow(i) && check_board->checkLower(i, Combined))
				|| (board->canGetLeftColumn(i) && check_board->checkLeft(i, Combined))) {
				checkConnectionOther(i, chain_count, board);
			}
		}
	}
}

int Chain_Info::recursionCheckConnection(const int i, Board *board, Check_Board *check_board, int count)
{
	// 確認中
	check_board->setBoardElementChecking(i);
	// 結合カウント+1
	++count;
	// 上がある
	if (check_board->canGetUpperRow(i)
		&& check_board->checkUpper(i, NoCheck)
		&& board->isSameUpper(i)) {
		count = recursionCheckConnection(i - column_size, board, check_board, count);
	}
	// 右がある
	if (check_board->canGetRightColumn(i)
		&& check_board->checkRight(i, NoCheck)
		&& board->isSameRight(i)) {
		count = recursionCheckConnection(i + 1, board, check_board, count);
	}
	// 下がある
	if (check_board->canGetLowerRow(i)
		&& check_board->checkLower(i, NoCheck)
		&& board->isSameLower(i)) {
		count = recursionCheckConnection(i + column_size, board, check_board, count);
	}
	// 左がある
	if (check_board->canGetLeftColumn(i)
		&& check_board->checkLeft(i, NoCheck)
		&& board->isSameLeft(i)) {
		count = recursionCheckConnection(i - 1, board, check_board, count);
	}
	return count;
}

void Chain_Info::checkConnectionOther(const int i, const int chain_count, Board *board)
{
	int color = board->getBoardElement(i);
	switch (color)
	{
	case Ojama:
		board->setBoardElement(i, Elimination);
		break;
	case Kata:
		board->setBoardElement(i, Ojama);
		break;
	case Heart:
		board->setBoardElement(i, Elimination);
		break;
	case Prism:
		board->setBoardElement(i, Elimination);
		break;
	default:
		break;
	}
	setElementCount(color, chain_count, 1);
}

bool Chain_Info::dropNext(Next* next, Board* board)
{
	int i;
	bool drop_next_flag = false;
	for (i = 0; i < board_size; ++i) {
		if (!board->isNone(end_of_board - i)) {
			continue;
		}
		if (!next->isNone((end_of_board - i) % column_size)) {
			board->setBoardElement(end_of_board - i, next->getNextElement((end_of_board - i) % column_size));
			next->setNextElement((end_of_board - i) % column_size, None);
			drop_next_flag = true;
		}
	}
	return drop_next_flag;
}

void Chain_Info::setElementCount(const int color, const int chain_count, const int count)
{
	switch (color)
	{
	case Red:
		all_chain_info[chain_count].num_r += count;
		break;
	case Blue:
		all_chain_info[chain_count].num_b += count;
		break;
	case Green:
		all_chain_info[chain_count].num_g += count;
		break;
	case Yellow:
		all_chain_info[chain_count].num_y += count;
		break;
	case Purple:
		all_chain_info[chain_count].num_p += count;
		break;
	case Ojama:
		all_chain_info[chain_count].num_ojama += count;
		break;
	case Kata:
		all_chain_info[chain_count].num_kata += count;
		break;
	case Heart:
		all_chain_info[chain_count].num_heart += count;
		break;
	case Prism:
		all_chain_info[chain_count].num_prism += count;
	default:
		break;
	}
}

void Chain_Info::setSeparateCount(const int color, const int chain_count)
{
	switch (color)
	{
	case Red:
		all_chain_info[chain_count].sep_r += 1;
		break;
	case Blue:
		all_chain_info[chain_count].sep_b += 1;
		break;
	case Green:
		all_chain_info[chain_count].sep_g += 1;
		break;
	case Yellow:
		all_chain_info[chain_count].sep_y += 1;
		break;
	case Purple:
		all_chain_info[chain_count].sep_p += 1;
		break;
	default:
		break;
	}
}

void Chain_Info::debugPrint(const Param_Info *pi, const Next* next, const Board* board, const std::string str) const
{
	if (pi->isProcessPrint()) {
		std::cout << str << std::endl;
		next->print();
		std::cout << "--------" << std::endl;
		board->print();
		std::cout << std::endl;
	}
}

void Chain_Info::debugPrint(const Param_Info* pi, const Board* board, const std::string str) const
{
	if (pi->isProcessPrint()) {
		std::cout << str << std::endl;
		std::cout << "[ none ]" << std::endl;
		std::cout << "--------" << std::endl;
		board->print();
		std::cout << std::endl;
	}
}

void Chain_Info::debugChain(const int chain_count) const
{
	std::cout << chain_count << "連鎖目" << std::endl;
	std::cout << "赤個数:" << getElementCount(Red, chain_count) << " 赤分離数:" << getSeparateCount(Red, chain_count) << std::endl;
	std::cout << "青個数:" << getElementCount(Blue, chain_count) << " 青分離数:" << getSeparateCount(Blue, chain_count) << std::endl;
	std::cout << "緑個数:" << getElementCount(Green, chain_count) << " 緑分離数:" << getSeparateCount(Green, chain_count) << std::endl;
	std::cout << "黄個数:" << getElementCount(Yellow, chain_count) << " 黄分離数:" << getSeparateCount(Yellow, chain_count) << std::endl;
	std::cout << "紫個数:" << getElementCount(Purple, chain_count) << " 紫分離数:" << getSeparateCount(Purple, chain_count) << std::endl;
	std::cout << "邪個数:" << getElementCount(Ojama, chain_count) << " 固個数:" << getElementCount(Kata, chain_count) << " ハ個数:" << getElementCount(Heart, chain_count) << " プ個数:" << getElementCount(Prism, chain_count) << std::endl;
	std::cout << std::endl;
}

int Chain_Info::getElementCount(const int color, const int chain_count) const
{
	int count = 0;
	switch (color)
	{
	case Red:
		count = all_chain_info[chain_count - 1].num_r;
		break;
	case Blue:
		count = all_chain_info[chain_count - 1].num_b;
		break;
	case Green:
		count = all_chain_info[chain_count - 1].num_g;
		break;
	case Yellow:
		count = all_chain_info[chain_count - 1].num_y;
		break;
	case Purple:
		count = all_chain_info[chain_count - 1].num_p;
		break;
	case Ojama:
		count = all_chain_info[chain_count - 1].num_ojama;
		break;
	case Kata:
		count = all_chain_info[chain_count - 1].num_kata;
		break;
	case Heart:
		count = all_chain_info[chain_count - 1].num_heart;
		break;
	case Prism:
		count = all_chain_info[chain_count - 1].num_prism;
		break;
	default:
		break;
	}
	return count;
}

int Chain_Info::getSeparateCount(const int color, const int chain_count) const
{
	int count = 0;
	switch (color)
	{
	case Red:
		count = all_chain_info[chain_count - 1].sep_r;
		break;
	case Blue:
		count = all_chain_info[chain_count - 1].sep_b;
		break;
	case Green:
		count = all_chain_info[chain_count - 1].sep_g;
		break;
	case Yellow:
		count = all_chain_info[chain_count - 1].sep_y;
		break;
	case Purple:
		count = all_chain_info[chain_count - 1].sep_p;
		break;
	default:
		break;
	}
	return count;
}