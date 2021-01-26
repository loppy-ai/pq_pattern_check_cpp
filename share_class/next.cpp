#include "next.h"

Next::Next()
{
	next = next_entity;
}

Next::~Next()
{

}

int* Next::getNext() const
{
	return next;
}

void Next::setNext(const int* input_next)
{
	int i;
	for (i = 0; i < next_size; ++i)
	{
		next[i] = input_next[i];
	}
}

int Next::getNextElement(const int no) const
{
	return next[no];
}

void Next::setNextElement(const int no, const int val)
{
	next[no] = val;
}

bool Next::isNone(const int no) const
{
	bool none_flag;
	if (getNextElement(no) == None) {
		none_flag = true;
	}
	else {
		none_flag = false;
	}
	return none_flag;
}

void Next::print() const
{
	int i;
	std::string str_next[column_size];
	for (i = 0; i < column_size; ++i) {
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

void Next::print(const std::string title) const
{
	std::cout << title << std::endl;
	Next::print();
}