#include "chain_info.h"

Chain_Info::Chain_Info(const Param_Info* pi, Next* next, Board* board, const Board* trace_pattern_board)
{
	chain(pi, next, board, trace_pattern_board);
}

Chain_Info::~Chain_Info()
{

}

// 連鎖処理
void Chain_Info::chain(const Param_Info* pi, Next* next, Board* board, const Board* trace_pattern_board)
{
	bool chaining_flag = true;
#ifdef SW_NEXT_FELL_END
	bool next_fell_flag = false;
#endif // SW_NEXT_FELL_END
#ifdef SW_DISP_RESULT
	if (pi->isProcessPrint()) {
	    std::cout << "---------------------連鎖過程---------------------" << std::endl;
	}
	debugPrint(pi, next, board, "default");
	debugPrint(pi, trace_pattern_board, "tracePattern");
#endif	// SW_DISP_RESULT
	if ((pi->getBoardPattern() > 200) && (pi->getBoardPattern() < 300)) {
		// しろいマール盤面
		applyTracePatternBlue(board, trace_pattern_board);
	}
	else if ((pi->getBoardPattern() > 300) && (pi->getBoardPattern() < 400)) {
		// あたり＆プーボ盤面
		applyTracePatternYellow(board, trace_pattern_board);
	}
	else {
		// 盤面に対してなぞり消しパターンを適用
		applyTracePattern(board, trace_pattern_board);
	}
#ifdef SW_DISP_RESULT
	debugPrint(pi, next, board, "applyedTracePattern");
#endif	// SW_DISP_RESULT
	// 消えた部分を落とす
	dropBoard(board);
#ifdef SW_DISP_RESULT
	debugPrint(pi, next, board, "dropBoard");
#endif	// SW_DISP_RESULT

	// 連鎖数分ループ
	for (int chain_count = 0; chain_count <= max_num_of_chain; /* no-increment */ ) {
		// 結合チェック用盤面インスタンスの生成
		Check_Board check_board;
		// 結合チェック
		checkConnection(chain_count, pi, board, &check_board);
		if (board->isEliminationHappened()) {
			// ぷよが消えたら次の連鎖
			++chain_count;
#ifdef SW_DISP_RESULT
			debugPrint(pi, next, board, "elimination");
			debugChain(pi, chain_count);
#endif	// SW_DISP_RESULT
			dropBoard(board);
#ifdef SW_DISP_RESULT
			debugPrint(pi, next, board, "dropBoard");
#endif	// SW_DISP_RESULT
#ifdef SW_NEXT_FELL_END
			// nextが落ちたことによるぷよ消しであれば終了とする
			if (next_fell_flag) {
				num_of_chain = chain_count;
				break;
			}
#endif // SW_NEXT_FELL_END
		}
		else {
			// ぷよが消えなかったらnextを落とす
			if (!dropNext(next, board)) {
				// nextが落ちなくなったら連鎖終了
				chaining_flag = false;
			}
#ifdef SW_DISP_RESULT
			debugPrint(pi, next, board, "dropNext");
#endif	// SW_DISP_RESULT
#ifdef SW_NEXT_FELL_END
			// nextが落ちた
			next_fell_flag = true;
#endif // SW_NEXT_FELL_END
		}
		if (chaining_flag == false) {
			num_of_chain = chain_count;
			break;
		}
	}
}

// なぞり消しパターンを適用
void Chain_Info::applyTracePattern(Board* board, const Board* trace_pattern_board) {
	for (int i = 0; i < board_size; ++i) {
		if (trace_pattern_board->getBoardElement(i) == 1) {
			board->setBoardElement(i, Elimination);
		}
	}
}

// なぞり消しパターンを適用（しろいマール用）
void Chain_Info::applyTracePatternBlue(Board* board, const Board* trace_pattern_board) {
	for (int i = 0; i < board_size; ++i) {
		if (trace_pattern_board->getBoardElement(i) == 1) {
			board->setBoardElement(i, Blue);
		}
	}
}

// なぞり消しパターンを適用（あたり＆プーボ用）
void Chain_Info::applyTracePatternYellow(Board* board, const Board* trace_pattern_board) {
	for (int i = 0; i < board_size; ++i) {
		if (trace_pattern_board->getBoardElement(i) == 1) {
			board->setBoardElement(i, Yellow);
		}
	}
}

// 消えたぷよの上にあるものを落とす
void Chain_Info::dropBoard(Board* board)
{
	// 0:なぞったor結合して消えた 9:落ちて消えた
	// 盤面走査
	for (int i = 0; i < board_size; ++i) {
		// 0以外は次を見る
		if (board->getBoardElement(end_of_board - i) != Elimination) {
			continue;
		}
		// 0だったら1つ上を見る
		for (int target = end_of_board - i - column_size; target >= 0; target -= column_size) {
			if (board->getBoardElement(target) != Elimination) {
				// 1つ上が0以外だったら落として、1つ上を0にする
				board->setBoardElement(end_of_board - i, board->getBoardElement(target));
				board->setBoardElement(target, Elimination);
				break;
			}
		}
	}
	// 落ちて消えたところを9にする
	for (int i = 0; i < board_size; ++i) {
		if (board->getBoardElement(i) == 0) {
			board->setBoardElement(i, None);
		}
	}
}

// 結合チェック
void Chain_Info::checkConnection(const int chain_count, const Param_Info* pi, Board* board, Check_Board* check_board)
{
	const int max_connection = pi->getMaxConnection();

	// 色ぷよ以外はチェック済(未結合)にする
	for (int i = 0; i < board_size; ++i) {
		if (!(board->isColorPuyo(i))) {
			check_board->setBoardElementUncombined(i);
		}
	}
	// 結合チェック
	for (int i = 0; i < board_size; ++i) {
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
			// 情報格納
			setElementCount(board->getBoardElement(i), chain_count, count);
			setSeparateCount(board->getBoardElement(i), chain_count);
			for (int j = 0; j < board_size; ++j) {
				if (check_board->isChecking(j)) {
					check_board->setBoardElementCombined(j);
				}
			}
		}
		// 消える数だけ繋がっていなかった場合
		else {
			for (int j = 0; j < board_size; ++j) {
				if (check_board->isChecking(j)) {
					check_board->setBoardElementUncombined(j);
				}
			}
		}
	}
	// 結合していたところの盤面を0にする
	for (int i = 0; i < board_size; ++i) {
		if (check_board->isCombined(i)) {
			board->setBoardElement(i, Elimination);
		}
	}
	// 周囲の影響を受けるものの処理
	for (int i = 0; i < board_size; ++i) {
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

// 結合チェック（再帰用）
int Chain_Info::recursionCheckConnection(const int i, Board* board, Check_Board* check_board, int count)
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

// 巻き込まれて消えるものの処理
void Chain_Info::checkConnectionOther(const int i, const int chain_count, Board* board)
{
	int color = board->getBoardElement(i);
	switch (color)
	{
	case Ojama:	board->setBoardElement(i, Elimination);	break;
	case Kata:	board->setBoardElement(i, Ojama);		break;	// かたぷよは消えるとおじゃまぷよになる
	case Heart:	board->setBoardElement(i, Elimination);	break;
	case Prism:	board->setBoardElement(i, Elimination);	break;
	default:											break;
	}
	setElementCount(color, chain_count, 1);
}

// ネクストを落とす
bool Chain_Info::dropNext(Next* next, Board* board)
{
	bool drop_next_flag = false;
	for (int i = 0; i < board_size; ++i) {
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

// ある色・ある連鎖での消去数を登録
void Chain_Info::setElementCount(const int color, const int chain_count, const int count)
{
	switch (color)
	{
	case Red:		all_chain_info[chain_count].num_r += count;		break;
	case Blue:		all_chain_info[chain_count].num_b += count;		break;
	case Green:		all_chain_info[chain_count].num_g += count;		break;
	case Yellow:	all_chain_info[chain_count].num_y += count;		break;
	case Purple:	all_chain_info[chain_count].num_p += count;		break;
	case Ojama:		all_chain_info[chain_count].num_ojama += count;	break;
	case Kata:		all_chain_info[chain_count].num_kata += count;	break;
	case Heart:		all_chain_info[chain_count].num_heart += count;	break;
	case Prism:		all_chain_info[chain_count].num_prism += count;	break;
	default:														break;
	}
}

// ある色・ある連鎖での分離数を登録
void Chain_Info::setSeparateCount(const int color, const int chain_count)
{
	switch (color)
	{
	case Red:		all_chain_info[chain_count].sep_r += 1;		break;
	case Blue:		all_chain_info[chain_count].sep_b += 1;		break;
	case Green:		all_chain_info[chain_count].sep_g += 1;		break;
	case Yellow:	all_chain_info[chain_count].sep_y += 1;		break;
	case Purple:	all_chain_info[chain_count].sep_p += 1;		break;
	default:													break;
	}
}

// 盤面表示
void Chain_Info::debugPrint(const Param_Info* pi, const Next* next, const Board* board, const std::string str) const
{
	if (pi->isProcessPrint()) {
		std::cout << str << std::endl;
		next->print();
		std::cout << "---------------" << std::endl;
		board->print();
		std::cout << std::endl;
	}
}

// 盤面表示
void Chain_Info::debugPrint(const Param_Info* pi, const Board* board, const std::string str) const
{
	if (pi->isProcessPrint()) {
		std::cout << str << std::endl;
		std::cout << "0 0 0 0 0 0 0 0" << std::endl;
		std::cout << "---------------" << std::endl;
		board->print();
		std::cout << std::endl;
	}
}

// 連鎖数ごとの情報表示
void Chain_Info::debugChain(const Param_Info* pi, const int chain_count) const
{
	if (pi->isProcessPrint()) {
		int cc = chain_count - 1;
		std::cout << chain_count << "連鎖目" << std::endl;
		std::cout << "赤個数:" << getElementCountPerColorAndChain(Red, cc) << " 赤分離:" << getSeparateCountPerColorAndChain(Red, cc) << " 赤倍率:" << getMagnificationPerColorAndChain(pi, Red, cc) << std::endl;
		std::cout << "青個数:" << getElementCountPerColorAndChain(Blue, cc) << " 青分離:" << getSeparateCountPerColorAndChain(Blue, cc) << " 青倍率:" << getMagnificationPerColorAndChain(pi, Blue, cc) << std::endl;
		std::cout << "緑個数:" << getElementCountPerColorAndChain(Green, cc) << " 緑分離:" << getSeparateCountPerColorAndChain(Green, cc) << " 緑倍率:" << getMagnificationPerColorAndChain(pi, Green, cc) << std::endl;
		std::cout << "黄個数:" << getElementCountPerColorAndChain(Yellow, cc) << " 黄分離:" << getSeparateCountPerColorAndChain(Yellow, cc) << " 黄倍率:" << getMagnificationPerColorAndChain(pi, Yellow, cc) << std::endl;
		std::cout << "紫個数:" << getElementCountPerColorAndChain(Purple, cc) << " 紫分離:" << getSeparateCountPerColorAndChain(Purple, cc) << " 紫倍率:" << getMagnificationPerColorAndChain(pi, Purple, cc) << std::endl;
		std::cout << "ワ個数:" << getElementCountPerChain(cc) << " ワ分離:" << getSeparateCountPerChain(cc) << " ワ倍率:" << getMagnificationPerColorAndChain(pi, None, cc) << std::endl;
		std::cout << "邪個数:" << getElementCountPerColorAndChain(Ojama, cc) << " 固個数:" << getElementCountPerColorAndChain(Kata, cc) << " ハ個数:" << getElementCountPerColorAndChain(Heart, cc) << " プ個数:" << getElementCountPerColorAndChain(Prism, cc) << std::endl;
		std::cout << std::endl;
	}
}

// ある色・ある連鎖での消去数を取得
int Chain_Info::getElementCountPerColorAndChain(const int color, const int chain_count) const
{
	switch (color)
	{
	case Red:		return all_chain_info[chain_count].num_r;		break;
	case Blue:		return all_chain_info[chain_count].num_b;		break;
	case Green:		return all_chain_info[chain_count].num_g;		break;
	case Yellow:	return all_chain_info[chain_count].num_y;		break;
	case Purple:	return all_chain_info[chain_count].num_p;		break;
	case Ojama:		return all_chain_info[chain_count].num_ojama;	break;
	case Kata:		return all_chain_info[chain_count].num_kata;	break;
	case Heart:		return all_chain_info[chain_count].num_heart;	break;
	case Prism:		return all_chain_info[chain_count].num_prism;	break;
	default:														break;
	}
	return 0;
}

// ある色・ある連鎖での分離数を取得
int Chain_Info::getSeparateCountPerColorAndChain(const int color, const int chain_count) const
{
	switch (color)
	{
	case Red:		return all_chain_info[chain_count].sep_r;	break;
	case Blue:		return all_chain_info[chain_count].sep_b;	break;
	case Green:		return all_chain_info[chain_count].sep_g;	break;
	case Yellow:	return all_chain_info[chain_count].sep_y;	break;
	case Purple:	return all_chain_info[chain_count].sep_p;	break;
	default:													break;
	}
	return 0;
}


// ある連鎖数での全分離数を取得
int Chain_Info::getSeparateCountPerChain(const int chain_count) const
{
	return getSeparateCountPerColorAndChain(Red, chain_count)
		+ getSeparateCountPerColorAndChain(Blue, chain_count)
		+ getSeparateCountPerColorAndChain(Green, chain_count)
		+ getSeparateCountPerColorAndChain(Yellow, chain_count)
		+ getSeparateCountPerColorAndChain(Purple, chain_count);
}

// ある色・ある連鎖での倍率を取得
double Chain_Info::getMagnificationPerColorAndChain(const Param_Info* pi, const int color, const int chain_count) const
{
	switch (color)
	{
	case Red:
	case Blue:
	case Green:
	case Yellow:
	case Purple:
		// (連鎖係数 * (1 + (同時に消した数 - 3or4) * 0.15 * 同時消し係数)) * 分離数 + 3 * プリボの数
		return (pi->getChainMagnification(chain_count) * (1 + (getElementCountPerChain(chain_count) - pi->getMaxConnection()) * 0.15 * pi->getEliminationCoefficient())) * getSeparateCountPerColorAndChain(color, chain_count) + 3 * getElementCountPerColorAndChain(Prism, chain_count);
		break;
	case None:	// ワイルド
		return (pi->getChainMagnification(chain_count) * (1 + (getElementCountPerChain(chain_count) - pi->getMaxConnection()) * 0.15 * pi->getEliminationCoefficient())) * getSeparateCountPerChain(chain_count) + 3 * getElementCountPerColorAndChain(Prism, chain_count);
		break;
	default:
		break;
	}
	return 0.0;
}

// ある色・ある連鎖での倍率を取得
double Chain_Info::getMagnificationPerColorAndChain(const Param_Info* pi, const double elimination_coefficient, const double chain_coefficient, const int color, const int chain_count) const
{
	switch (color)
	{
	case Red:
	case Blue:
	case Green:
	case Yellow:
	case Purple:
		// (連鎖係数 * (1 + (同時に消した数 - 3or4) * 0.15 * 同時消し係数)) * 分離数 + 3 * プリボの数
		return (pi->getChainMagnification(chain_count, chain_coefficient) * (1 + (getElementCountPerChain(chain_count) - pi->getMaxConnection()) * 0.15 * elimination_coefficient)) * getSeparateCountPerColorAndChain(color, chain_count) + 3 * getElementCountPerColorAndChain(Prism, chain_count);
		break;
	case None:	// ワイルド
		return (pi->getChainMagnification(chain_count, chain_coefficient) * (1 + (getElementCountPerChain(chain_count) - pi->getMaxConnection()) * 0.15 * elimination_coefficient)) * getSeparateCountPerChain(chain_count) + 3 * getElementCountPerColorAndChain(Prism, chain_count);
		break;
	default:
		break;
	}
	return 0.0;
}

// 最大連鎖数を取得
int Chain_Info::getNumOfChain() const
{
	return num_of_chain;
}

// ある色の全消去数を取得
int Chain_Info::getElementCountPerColor(const int color) const
{
	int sum = 0;
	for (int i = 0; i < getNumOfChain(); ++i) {
		sum += getElementCountPerColorAndChain(color, i);
	}
	return sum;
}

// ある連鎖の全消去数を取得
int Chain_Info::getElementCountPerChain(const int chain_count) const
{
	return getElementCountPerColorAndChain(Red, chain_count)
		+ getElementCountPerColorAndChain(Blue, chain_count)
		+ getElementCountPerColorAndChain(Green, chain_count)
		+ getElementCountPerColorAndChain(Yellow, chain_count)
		+ getElementCountPerColorAndChain(Purple, chain_count)
		+ getElementCountPerColorAndChain(Ojama, chain_count)
		+ getElementCountPerColorAndChain(Kata, chain_count);
}

// ある色の合計倍率を取得
double Chain_Info::getMagnificationPerColor(const Param_Info* pi, const int color) const
{
	double sum = 0.0;
	for (int i = 0; i < getNumOfChain(); ++i) {
		sum += getMagnificationPerColorAndChain(pi, color, i);
	}
	return sum;
}

// ある色の合計倍率を取得
double Chain_Info::getMagnificationPerColor(const Param_Info* pi, const double elimination_coefficient, const double chain_coefficient, const int color) const
{
	double sum = 0.0;
	for (int i = 0; i < getNumOfChain(); ++i) {
		sum += getMagnificationPerColorAndChain(pi, elimination_coefficient, chain_coefficient, color, i);
	}
	return sum;
}