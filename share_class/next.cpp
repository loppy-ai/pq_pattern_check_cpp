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
	std::cout
		<< getNextElement(0)
		<< getNextElement(1)
		<< getNextElement(2)
		<< getNextElement(3)
		<< getNextElement(4)
		<< getNextElement(5)
		<< getNextElement(6)
		<< getNextElement(7)
		<< std::endl;
}

void Next::print(const std::string title) const
{
	std::cout << title << std::endl;
	Next::print();
}