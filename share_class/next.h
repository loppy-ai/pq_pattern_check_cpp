#pragma once
#include <iostream>
#include "size.h"
#include "element.h"

class Next
{
private:
	int next_entity[next_size] = { 0 };

public:
	int* next;
	Next();
	~Next();
	int* getNext() const;
	void setNext(const int*);
	int getNextElement(const int) const;
	void setNextElement(const int, const int);
	bool isNone(const int) const;
	void print() const;
	void print(const std::string) const;
};