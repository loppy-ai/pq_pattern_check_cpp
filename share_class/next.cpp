#include "next.h"

Next::Next()
{
	next = next_entity;
}

Next::~Next()
{

}

// ネクスト全てをある色に設定
void Next::setNext(const int* input_next)
{
	std::memcpy(next, input_next, next_size * 4);	// int:4byte
}

// ネクストの要素を取得
int Next::getNextElement(const int no) const
{
	return next[no];
}

// ネクストの要素を設定
void Next::setNextElement(const int no, const int val)
{
	next[no] = val;
}

// ネクストがあるか
bool Next::isNone(const int no) const
{
	if (getNextElement(no) == None)	return true;
	return false;
}

// 盤面表示
void Next::print() const
{
	std::string str_next[column_size];
	for (int i = 0; i < column_size; ++i) {
		if (getNextElement(i) == None) {
			str_next[i] = ".";
		}
		else {
			str_next[i] = std::to_string(getNextElement(i));
		}
	}
	std::cout
		<< str_next[0] << " "
		<< str_next[1] << " "
		<< str_next[2] << " "
		<< str_next[3] << " "
		<< str_next[4] << " "
		<< str_next[5] << " "
		<< str_next[6] << " "
		<< str_next[7] << " "
		<< std::endl;
}