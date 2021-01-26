#include "chain_info.h"

Chain_Info::Chain_Info(const Param_Info* pi, Next* next, Board* board, const Board* trace_pattern_board)
{
	chain(pi, next, board, trace_pattern_board);
}

Chain_Info::~Chain_Info()
{

}

// �A������
void Chain_Info::chain(const Param_Info* pi, Next* next, Board* board, const Board* trace_pattern_board)
{
	bool chaining_flag = true;
	int chain_count;
	std::cout << "---------------------�A���ߒ�---------------------" << std::endl;
	debugPrint(pi, next, board, "default");
	debugPrint(pi, trace_pattern_board, "tracePattern");
	// �Ֆʂɑ΂��ĂȂ�������p�^�[����K�p
	applyTracePattern(board, trace_pattern_board);
	debugPrint(pi, next, board, "applyedTracePattern");
	// �����������𗎂Ƃ�
	dropBoard(board);
	debugPrint(pi, next, board, "dropBoard");

	// �A���������[�v
	for (chain_count = 0; chain_count <= max_num_of_chain; /* no-increment */ ) {
		// �����`�F�b�N�p�ՖʃC���X�^���X�̐���
		Check_Board check_board;
		// �����`�F�b�N
		checkConnection(chain_count, pi, board, &check_board);
		if (board->isEliminationHappened()) {
			// �Ղ悪�������玟�̘A��
			++chain_count;
			debugPrint(pi, next, board, "elimination");
			debugChain(chain_count);
			dropBoard(board);
			debugPrint(pi, next, board, "dropBoard");
		}
		else {
			// �Ղ悪�����Ȃ�������next�𗎂Ƃ�
			bool drop_next_flag;
			drop_next_flag = dropNext(next, board);
			debugPrint(pi, next, board, "dropNext");
			if (!drop_next_flag) {
				// next�������Ȃ��Ȃ�����A���I��
				chaining_flag = false;
			}
		}
		if (chaining_flag == false) {
			num_of_chain = chain_count;
			break;
		}
	}
}

// �Ȃ�������p�^�[����K�p
void Chain_Info::applyTracePattern(Board* board, const Board* trace_pattern_board) {
	int i;
	for (i = 0; i < board_size; ++i) {
		if (trace_pattern_board->getBoardElement(i) == 1) {
			board->setBoardElement(i, Elimination);
		}
	}
}

// �������Ղ�̏�ɂ�����̂𗎂Ƃ�
void Chain_Info::dropBoard(Board* board)
{
	// 0:�Ȃ�����or�������ď����� 9:�����ď�����
	int i, target;
	// �Ֆʑ���
	for (i = 0; i < board_size; ++i) {
		// 0�ȊO�͎�������
		if (board->getBoardElement(end_of_board - i) != Elimination) {
			continue;
		}
		// 0��������1�������
		for (target = end_of_board - i - column_size; target >= 0; target -= column_size) {
			if (board->getBoardElement(target) != Elimination) {
				// 1�オ0�ȊO�������痎�Ƃ��āA1���0�ɂ���
				board->setBoardElement(end_of_board - i, board->getBoardElement(target));
				board->setBoardElement(target, Elimination);
				break;
			}
		}
	}
	// �����ď������Ƃ����9�ɂ���
	for (i = 0; i < board_size; ++i) {
		if (board->getBoardElement(i) == 0) {
			board->setBoardElement(i, None);
		}
	}
}

// �����`�F�b�N
void Chain_Info::checkConnection(const int chain_count, const Param_Info* pi, Board* board, Check_Board* check_board)
{
	int i, j;
	// �F�Ղ�ȊO�̓`�F�b�N��(������)�ɂ���
	for (i = 0; i < board_size; ++i) {
		if (!board->isColorPuyo(i)) {
			check_board->setBoardElementUncombined(i);
		}
	}
	// �����`�F�b�N
	for (i = 0; i < board_size; ++i) {
		// �m�肵�Ă������΂�
		if (check_board->isConfirmed(i)) {
			continue;
		}
		// ������
		int count = 0;
		// �ċA�����`�F�b�N
		count = recursionCheckConnection(i, board, check_board, count);
		// i�Ԗڂ̂Ղ�͉��q�����Ă��邩���Ԃ��Ă���
		// �����鐔�����q�����Ă���ꍇ
		if (count >= pi->getMaxConnection()) {
			// all_chain_info�ɏ��i�[
			setElementCount(board->getBoardElement(i), chain_count, count);
			setSeparateCount(board->getBoardElement(i), chain_count);
			for (j = 0; j < board_size; ++j) {
				if (check_board->isChecking(j)) {
					check_board->setBoardElementCombined(j);
				}
			}
		}
		// �����鐔�����q�����Ă��Ȃ������ꍇ
		else {
			for (j = 0; j < board_size; ++j) {
				if (check_board->isChecking(j)) {
					check_board->setBoardElementUncombined(j);
				}
			}
		}
	}
	// �������Ă����Ƃ���̔Ֆʂ�0�ɂ���
	for (i = 0; i < board_size; ++i) {
		if (check_board->isCombined(i)) {
			board->setBoardElement(i, Elimination);
		}
	}
	// ���͂̉e�����󂯂���̂̏���
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

// �����`�F�b�N�i�ċA�p�j
int Chain_Info::recursionCheckConnection(const int i, Board* board, Check_Board* check_board, int count)
{
	// �m�F��
	check_board->setBoardElementChecking(i);
	// �����J�E���g+1
	++count;
	// �オ����
	if (check_board->canGetUpperRow(i)
		&& check_board->checkUpper(i, NoCheck)
		&& board->isSameUpper(i)) {
		count = recursionCheckConnection(i - column_size, board, check_board, count);
	}
	// �E������
	if (check_board->canGetRightColumn(i)
		&& check_board->checkRight(i, NoCheck)
		&& board->isSameRight(i)) {
		count = recursionCheckConnection(i + 1, board, check_board, count);
	}
	// ��������
	if (check_board->canGetLowerRow(i)
		&& check_board->checkLower(i, NoCheck)
		&& board->isSameLower(i)) {
		count = recursionCheckConnection(i + column_size, board, check_board, count);
	}
	// ��������
	if (check_board->canGetLeftColumn(i)
		&& check_board->checkLeft(i, NoCheck)
		&& board->isSameLeft(i)) {
		count = recursionCheckConnection(i - 1, board, check_board, count);
	}
	return count;
}

// �������܂�ď�������̂̏���
void Chain_Info::checkConnectionOther(const int i, const int chain_count, Board* board)
{
	int color = board->getBoardElement(i);
	switch (color)
	{
	case Ojama:
		board->setBoardElement(i, Elimination);
		break;
	case Kata:
		// ������Ƃ��ז��ɂȂ�
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

// �l�N�X�g�𗎂Ƃ�
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

// ����F�E����A���ł̏�������o�^
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

// ����F�E����A���ł̕�������o�^
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

// �Ֆʕ\��
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

// �Ֆʕ\��
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

// �A�������Ƃ̏��\��
void Chain_Info::debugChain(const int chain_count) const
{
	int cc = chain_count - 1;
	std::cout << chain_count << "�A����" << std::endl;
	std::cout << "�Ԍ�:" << getElementCountPerColorAndChain(Red, cc) << " �ԕ���:" << getSeparateCountPerColorAndChain(Red, cc) << std::endl;
	std::cout << "��:" << getElementCountPerColorAndChain(Blue, cc) << " ����:" << getSeparateCountPerColorAndChain(Blue, cc) << std::endl;
	std::cout << "�Ό�:" << getElementCountPerColorAndChain(Green, cc) << " �Ε���:" << getSeparateCountPerColorAndChain(Green, cc) << std::endl;
	std::cout << "����:" << getElementCountPerColorAndChain(Yellow, cc) << " ������:" << getSeparateCountPerColorAndChain(Yellow, cc) << std::endl;
	std::cout << "����:" << getElementCountPerColorAndChain(Purple, cc) << " ������:" << getSeparateCountPerColorAndChain(Purple, cc) << std::endl;
	std::cout << "�׌�:" << getElementCountPerColorAndChain(Ojama, cc) << " �Ō�:" << getElementCountPerColorAndChain(Kata, cc) << " �n��:" << getElementCountPerColorAndChain(Heart, cc) << " �v��:" << getElementCountPerColorAndChain(Prism, cc) << std::endl;
	std::cout << std::endl;
}

// ����F�E����A���ł̏��������擾
int Chain_Info::getElementCountPerColorAndChain(const int color, const int chain_count) const
{
	int count = 0;
	switch (color)
	{
	case Red:
		count = all_chain_info[chain_count].num_r;
		break;
	case Blue:
		count = all_chain_info[chain_count].num_b;
		break;
	case Green:
		count = all_chain_info[chain_count].num_g;
		break;
	case Yellow:
		count = all_chain_info[chain_count].num_y;
		break;
	case Purple:
		count = all_chain_info[chain_count].num_p;
		break;
	case Ojama:
		count = all_chain_info[chain_count].num_ojama;
		break;
	case Kata:
		count = all_chain_info[chain_count].num_kata;
		break;
	case Heart:
		count = all_chain_info[chain_count].num_heart;
		break;
	case Prism:
		count = all_chain_info[chain_count].num_prism;
		break;
	default:
		break;
	}
	return count;
}

// ����F�E����A���ł̕��������擾
int Chain_Info::getSeparateCountPerColorAndChain(const int color, const int chain_count) const
{
	int count = 0;
	switch (color)
	{
	case Red:
		count = all_chain_info[chain_count].sep_r;
		break;
	case Blue:
		count = all_chain_info[chain_count].sep_b;
		break;
	case Green:
		count = all_chain_info[chain_count].sep_g;
		break;
	case Yellow:
		count = all_chain_info[chain_count].sep_y;
		break;
	case Purple:
		count = all_chain_info[chain_count].sep_p;
		break;
	default:
		break;
	}
	return count;
}


// ����A�����ł̑S���������擾
int Chain_Info::getSeparateCountPerChain(const int chain_count) const
{
	int count = 0;
	count = getSeparateCountPerColorAndChain(Red, chain_count)
		+ getSeparateCountPerColorAndChain(Blue, chain_count)
		+ getSeparateCountPerColorAndChain(Green, chain_count)
		+ getSeparateCountPerColorAndChain(Yellow, chain_count)
		+ getSeparateCountPerColorAndChain(Purple, chain_count);
	return count;
}

// ����F�E����A���ł̔{�����擾
double Chain_Info::getMagnificationPerColorAndChain(const Param_Info* pi, const int color, const int chain_count) const
{
	double magnification = 0.0;
	switch (color)
	{
	case Red:
	case Blue:
	case Green:
	case Yellow:
	case Purple:
		// (�A���W�� * (1 + (�����ɏ������� - 3or4) * 0.15 * ���������W��)) * ������ + 3 * �v���{�̐�
		magnification = (pi->getChainMagnification(chain_count) * (1 + (getElementCountPerChain(chain_count) - pi->getMaxConnection()) * 0.15 * pi->getEliminationCoefficient())) * getSeparateCountPerColorAndChain(color, chain_count) + 3 * getElementCountPerColorAndChain(Prism, chain_count);
		break;
	case None:	// ���C���h
		magnification = (pi->getChainMagnification(chain_count) * (1 + (getElementCountPerChain(chain_count) - pi->getMaxConnection()) * 0.15 * pi->getEliminationCoefficient())) * getSeparateCountPerChain(chain_count) + 3 * getElementCountPerColorAndChain(Prism, chain_count);
		break;
	default:
		break;
	}
	return magnification;
}

// ����F�E����A���ł̔{�����擾
double Chain_Info::getMagnificationPerColorAndChain(const Param_Info* pi, const double elimination_coefficient, const double chain_coefficient, const int color, const int chain_count) const
{
	double magnification = 0.0;
	switch (color)
	{
	case Red:
	case Blue:
	case Green:
	case Yellow:
	case Purple:
		// (�A���W�� * (1 + (�����ɏ������� - 3or4) * 0.15 * ���������W��)) * ������ + 3 * �v���{�̐�
		magnification = (pi->getChainMagnification(chain_count, chain_coefficient) * (1 + (getElementCountPerChain(chain_count) - pi->getMaxConnection()) * 0.15 * elimination_coefficient)) * getSeparateCountPerColorAndChain(color, chain_count) + 3 * getElementCountPerColorAndChain(Prism, chain_count);
		break;
	case None:	// ���C���h
		magnification = (pi->getChainMagnification(chain_count, chain_coefficient) * (1 + (getElementCountPerChain(chain_count) - pi->getMaxConnection()) * 0.15 * elimination_coefficient)) * getSeparateCountPerChain(chain_count) + 3 * getElementCountPerColorAndChain(Prism, chain_count);
		break;
	default:
		break;
	}
	return magnification;
}

// �ő�A�������擾
int Chain_Info::getNumOfChain() const
{
	return num_of_chain;
}

// ����F�̑S���������擾
int Chain_Info::getElementCountPerColor(const int color) const
{
	int i;
	int sum = 0;
	for (i = 0; i < getNumOfChain(); ++i) {
		sum += getElementCountPerColorAndChain(color, i);
	}
	return sum;
}

// ����A���̑S���������擾
int Chain_Info::getElementCountPerChain(const int chain_count) const
{
	int sum;
	sum = getElementCountPerColorAndChain(Red, chain_count)
		+ getElementCountPerColorAndChain(Blue, chain_count)
		+ getElementCountPerColorAndChain(Green, chain_count)
		+ getElementCountPerColorAndChain(Yellow, chain_count)
		+ getElementCountPerColorAndChain(Purple, chain_count)
		+ getElementCountPerColorAndChain(Ojama, chain_count)
		+ getElementCountPerColorAndChain(Kata, chain_count);
	return sum;
}

// ����F�̍��v�{�����擾
double Chain_Info::getMagnificationPerColor(const Param_Info* pi, const int color) const
{
	int i;
	double sum = 0.0;
	for (i = 0; i < getNumOfChain(); ++i) {
		sum += getMagnificationPerColorAndChain(pi, color, i);
	}
	return sum;
}

// ����F�̍��v�{�����擾
double Chain_Info::getMagnificationPerColor(const Param_Info* pi, const double elimination_coefficient, const double chain_coefficient, const int color) const
{
	int i;
	double sum = 0.0;
	for (i = 0; i < getNumOfChain(); ++i) {
		sum += getMagnificationPerColorAndChain(pi, elimination_coefficient, chain_coefficient, color, i);
	}
	return sum;
}